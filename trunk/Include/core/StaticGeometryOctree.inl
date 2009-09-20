#ifndef _STATIC_GEOMETRY_OCTREE_H
#error "This file can only be included from StaticGeometryOctree.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename Elem, typename RGS>
StaticGeometryOctree<Elem, RGS>::StaticGeometryOctree(
                                          const AABoundingBox& treeBB, 
                                          int maxElements,
                                          int maxDepth,
                                          RGS& outputGeometry,
                                          int initDepth)
: Octree(treeBB)
, m_elements(65536)
, m_maxElemsPerSector(maxElements)
, m_maxDepth(maxDepth)
, m_outputGeometry(outputGeometry)
{
   if (m_maxElemsPerSector <= 0)      {m_maxElemsPerSector = 1;}
   if (m_maxDepth < 0)                {m_maxDepth = 0;}
   if (initDepth > (int)m_maxDepth)   {initDepth = m_maxDepth;}

   subdivideTree(*m_root, initDepth);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem, typename RGS>
StaticGeometryOctree<Elem, RGS>::~StaticGeometryOctree()
{
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem, typename RGS>
bool StaticGeometryOctree<Elem, RGS>::isAdded(const Elem& elem) const
{
   unsigned int elemsCount = m_elements.size();
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      if (&elem == m_elements[i]) {return true;}
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem, typename RGS>
void StaticGeometryOctree<Elem, RGS>::insert(Elem& elem)
{
   if (isAdded(elem) == true) {return;}

   Array<Sector*> changedSectors;
   addElemToTree(elem, *m_root, changedSectors);

   Sector* sector = NULL;
   unsigned int sectorsCount = changedSectors.size();
   for (unsigned int i = 0; i < sectorsCount; ++i)
   {
      sector = changedSectors[i];
      if (  (sector->m_elems.size() > m_maxElemsPerSector) 
         && (sector->m_depth < m_maxDepth))
      {
         performSectorSubdivision(*sector);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem, typename RGS>
void StaticGeometryOctree<Elem, RGS>::performSectorSubdivision(Sector& sector)
{
   subdivideSector(sector);

   // perform the copy of elements used in this sector
   unsigned int elemsCount = sector.m_elems.size();
   Array<Sector*> changedSectors(elemsCount * 8);

   unsigned int elemIdx = 0;
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      elemIdx = sector.m_elems[i];
      Elem* elementToInsert = m_elements[elemIdx];
      m_freeSpaces.push(elemIdx);
      addElemToTree(*elementToInsert, sector, changedSectors);
   }

   sector.m_elems.clear();
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem, typename RGS>
void StaticGeometryOctree<Elem, RGS>::addElemToTree(Elem& elem, 
                                                    Sector& subTreeRoot,
                                                    Array<Sector*>& changedSectors)
{
   if (subTreeRoot.m_bb.testCollision(elem.getBoundingVolume()) == false) {return;}

   if (subTreeRoot.m_children != NULL)
   {
      // this is a composite - split the element across the sector's clipping
      // planes and try adding the resulting elements to the subtrees

      // generate the three split planes
      D3DXVECTOR3 bbMidPoint = (subTreeRoot.m_bb.min + subTreeRoot.m_bb.max) / 2.0f;
      D3DXPLANE planes[3];
      D3DXPlaneFromPointNormal(&planes[0], &bbMidPoint, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
      D3DXPlaneFromPointNormal(&planes[1], &bbMidPoint, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
      D3DXPlaneFromPointNormal(&planes[2], &bbMidPoint, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
      
      Array<Elem*> splitElems(16);
      splitElems.push_back(&elem);
      unsigned int elemStartIdx = 0;
      unsigned int elemEndIdx = 1;
      Elem* currElem = NULL;
      Elem* frontSplit = NULL;
      Elem* backSplit = NULL;
      for (unsigned int planeIdx = 0; planeIdx < 3; ++planeIdx)
      {
         for (unsigned int elemIdx = elemStartIdx; elemIdx < elemEndIdx; ++elemIdx)
         {
            currElem = splitElems[elemIdx];
            frontSplit = backSplit = NULL;   

            if (currElem != NULL)
            {
               float objPlaneClassification = currElem->getBoundingVolume().classifyAgainstPlane(planes[planeIdx]);

               if (objPlaneClassification < 0)
               {
                  backSplit = currElem;
               }
               else if (objPlaneClassification == 0)
               {
                  // object intersects the plane
                  currElem->split(planes[planeIdx], &frontSplit, &backSplit);
                  m_outputGeometry.unmanage(*currElem);
                  if (frontSplit != NULL) {m_outputGeometry.manage(frontSplit);}
                  if (backSplit != NULL) {m_outputGeometry.manage(backSplit);}
               }
               else
               {
                  // object is in front of the plane
                  frontSplit = currElem;
               }
            }

            splitElems.push_back(frontSplit);
            splitElems.push_back(backSplit);
         }
         elemStartIdx = elemEndIdx;
         elemEndIdx = splitElems.size();
      }

      for (unsigned int subSectorIdx = 0; subSectorIdx < 8; ++subSectorIdx)
      {
         Elem* elem = splitElems[subSectorIdx + 7];
         if (elem != NULL) 
         {
            addElemToTree(*elem, *(subTreeRoot.m_children[subSectorIdx]), changedSectors);
         }
      }
   }
   else
   {
      // this is a leaf - add the element to it
      unsigned int elemIdx = 0;
      if (m_freeSpaces.empty() == true)
      {
         elemIdx = m_elements.size();
         m_elements.push_back(&elem);
      }
      else
      {
         elemIdx = m_freeSpaces.pop();
         m_elements[elemIdx] = &elem;
      }
      subTreeRoot.m_elems.push_back(elemIdx);
      changedSectors.push_back(&subTreeRoot);
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem, typename RGS>
void StaticGeometryOctree<Elem, RGS>::remove(Elem& elem)
{
   unsigned int elemIdx = m_elements.find(&elem);
   if (elemIdx == EOA) {return;}
   m_elements[elemIdx] = NULL;
   m_freeSpaces.push(elemIdx);

   Array<Sector*> sectors;
   querySectors(elem.getBoundingVolume(), *m_root, sectors);

   unsigned int count = sectors.size();
   unsigned int idx;
   for (unsigned int i = 0; i < count; ++i)
   {
      idx = sectors[i]->m_elems.find(elemIdx);
      if (idx == EOA) {continue;}
      else {sectors[i]->m_elems.remove(idx);}
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem, typename RGS>
unsigned int StaticGeometryOctree<Elem, RGS>::getElementsCount() const
{
   return m_elements.size();
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem, typename RGS>
Elem& StaticGeometryOctree<Elem, RGS>::getElement(unsigned int idx) const
{
   return *(m_elements[idx]);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _STATIC_GEOMETRY_OCTREE_H
