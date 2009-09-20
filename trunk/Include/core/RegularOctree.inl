#ifndef _REGULAR_OCTREE_H
#error "This file can only be included from RegularOctree.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
RegularOctree<Elem>::RegularOctree(const AABoundingBox& treeBB, 
                                   unsigned int maxElemsPerSector,
                                   unsigned int maxTreeDepth)
: Octree(treeBB)
, m_elements(65536)
, m_maxElemsPerSector(maxElemsPerSector)
, m_maxTreeDepth(maxTreeDepth)
{
   if (m_maxElemsPerSector == 0) {m_maxElemsPerSector = 1;}
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
RegularOctree<Elem>::~RegularOctree()
{
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
bool RegularOctree<Elem>::isAdded(const Elem& elem) const
{
   unsigned int elemsCount = m_elements.size();
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      if (&elem == m_elements[i]) {return true;}
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void RegularOctree<Elem>::insert(Elem& elem)
{
   if (isAdded(elem) == true) {return;}

   // add the element to the collection of elements
   unsigned int elemIdx;
   if (m_freePos.empty() == true)
   {
      elemIdx = m_elements.size();
      m_elements.push_back(&elem);
   }
   else
   {
      elemIdx = m_freePos.pop();
      m_elements[elemIdx] = &elem;
   }

   // place the element id in the correct sector
   Array<Sector*> candidateSectors;
   unsigned int sectorsCount = 0;

   querySectors(elem.getBoundingVolume(), *m_root, candidateSectors);
   sectorsCount = candidateSectors.size();

   ASSERT(sectorsCount > 0, "The world is too small to add this element");

   Sector* sector = NULL;
   for (unsigned int i = 0; i < sectorsCount; ++i)
   {
      sector = candidateSectors[i];
      sector->m_elems.push_back(elemIdx);

      if ((sector->m_elems.size() > m_maxElemsPerSector) && 
         (sector->m_depth < m_maxTreeDepth))
      {
         subdivideSector(*sector);
         spreadChildren(*sector);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void RegularOctree<Elem>::remove(Elem& elem)
{
   // remove the element from the array of elements
   unsigned int removedElemIdx = m_elements.find(&elem);
   m_elements[removedElemIdx] = NULL;
   m_freePos.push(removedElemIdx);

   // step through the tree and remove all the references to this element
   Stack<Sector*> stack;
   stack.push(m_root);

   while(stack.empty() == false)
   {
      Sector* currSector = stack.pop();

      if (currSector->m_children != NULL)
      {
         ASSERT(currSector->m_elems.size() == 0, "Composite node has an element assigned");
         for (int i = 0; i < 8; ++i)
         {
            stack.push(currSector->m_children[i]);
         }
      }
      else
      {
         Array<unsigned int>& sectorElems = currSector->m_elems;
         unsigned int idx = sectorElems.find(removedElemIdx);
         if (idx != EOA) {sectorElems.remove(idx);}
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
unsigned int RegularOctree<Elem>::getElementsCount() const
{
   return m_elements.size();
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Elem& RegularOctree<Elem>::getElement(unsigned int idx) const
{
   return *(m_elements[idx]);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void RegularOctree<Elem>::spreadChildren(Sector& sector)
{
   // distribute the sector's elements among its children
   Array<unsigned int> elemsToDistribute(sector.m_elems);
   unsigned int elemsCount = elemsToDistribute.size();
   sector.m_elems.clear();

   Array<Sector*> candidateSectors;
   unsigned int sectorsCount = 0;
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      candidateSectors.clear();
      querySectors(m_elements[elemsToDistribute[i]]->getBoundingVolume(), sector, candidateSectors);

      sectorsCount = candidateSectors.size();
      for (unsigned int j = 0; j < sectorsCount; ++j)
      {
         candidateSectors[j]->m_elems.push_back(elemsToDistribute[i]);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _REGULAR_OCTREE_H
