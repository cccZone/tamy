#ifndef _STATIC_GEOMETRY_OCTREE_H
#error "This file can only be included from StaticGeometryOctree.h"
#else

#include <algorithm>
#include "core\Plane.h"


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
StaticGeometryOctree<Elem>::StaticGeometryOctree( const AABoundingBox& treeBB, int maxElements, int maxDepth, int initDepth )
   : Octree( treeBB )
   , m_elements( 65536 )
   , m_maxElemsPerSector( maxElements )
   , m_maxDepth( maxDepth )
{
   if (m_maxElemsPerSector <= 0)      {m_maxElemsPerSector = 1;}
   if (m_maxDepth < 0)                {m_maxDepth = 0;}
   if (initDepth > (int)m_maxDepth)   {initDepth = m_maxDepth;}

   subdivideTree(*m_root, initDepth);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
StaticGeometryOctree<Elem>::~StaticGeometryOctree()
{
   for ( ElementsArray::iterator it = m_elements.begin(); it != m_elements.end(); ++it )
   {
      delete *it;
   }
   m_elements.clear();

   for ( HandlesArray::iterator it = m_handles.begin(); it != m_handles.end(); ++it )
   {
      delete *it;
   }
   m_handles.clear();
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
SGHandle StaticGeometryOctree<Elem>::insert( Elem* elem )
{
   SGHandle handle = addHandle();
   SGElementParts& elemParts = *(m_handles[handle]);

   Array< Sector*, MemoryPoolAllocator > changedSectors( 16, m_allocator );
   addElemToTree( new SGElement(handle, elem), *m_root, changedSectors );

   Sector* sector = NULL;
   unsigned int sectorsCount = changedSectors.size();
   for ( unsigned int i = 0; i < sectorsCount; ++i )
   {
      sector = changedSectors[i];
      if ( ( sector->m_elems.size() > m_maxElemsPerSector ) && ( sector->getDepth() < m_maxDepth ) )
      {
         performSectorSubdivision(*sector);
      }
   }

   recalculateElementsBounds();

   return handle;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void StaticGeometryOctree<Elem>::performSectorSubdivision(Sector& sector)
{
   sector.subdivide();

   // perform the copy of elements used in this sector
   unsigned int childrenCount = sector.getChildrenCount();
   unsigned int elemsCount = sector.m_elems.size();
   Array< Sector*, MemoryPoolAllocator> changedSectors( elemsCount * childrenCount, m_allocator );

   unsigned int elemIdx = 0;
   SGElementParts* vecParts = NULL;
   for ( unsigned int i = 0; i < elemsCount; ++i )
   {
      elemIdx = sector.m_elems[i];
      SGElement* elementToInsert = releaseElement( elemIdx );

      vecParts = m_handles[elementToInsert->handle];
      SGElementParts::iterator sgElemPartIt = std::find( vecParts->begin(), vecParts->end(), elemIdx);
      if ( sgElemPartIt != vecParts->end() )
      {
         vecParts->erase(sgElemPartIt);
      }

      addElemToTree(elementToInsert, sector, changedSectors);
   }

   sector.m_elems.clear();
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void StaticGeometryOctree<Elem>::addElemToTree( SGElement* element, Sector& subTreeRoot, Array< Sector*, MemoryPoolAllocator >& changedSectors )
{
   if ( subTreeRoot.doesIntersect( element->elem->getBoundingVolume() ) == false ) 
   {
      return;
   }

   unsigned int childrenCount = subTreeRoot.getChildrenCount();
   if  (childrenCount > 0 )
   {
      // this is a composite - split the element across the sector's clipping
      // planes and try adding the resulting elements to the subtrees

      // generate the three split planes
      unsigned int splitPlanesCount = subTreeRoot.getSplitPlanesCount();
      ASSERT_MSG(splitPlanesCount > 0, "invalid number of split planes");
      ASSERT_MSG(splitPlanesCount <= 3, "invalid number of split planes");
      const Plane* planes[3];
      for (unsigned int i = 0; i < splitPlanesCount; ++i)
      {
         planes[i] = &subTreeRoot.getSplitPlane(i);
      }
      
      Array<SGElement*> splitElems(16);
      splitElems.push_back(element);
      unsigned int elemStartIdx = 0;
      unsigned int elemEndIdx = 1;
      SGElement* currElem = NULL;
      SGElement* frontSplit = NULL;
      SGElement* backSplit = NULL;
      for (unsigned int planeIdx = 0; planeIdx < splitPlanesCount; ++planeIdx)
      {
         for (unsigned int elemIdx = elemStartIdx; elemIdx < elemEndIdx; ++elemIdx)
         {
            currElem = splitElems[elemIdx];
            frontSplit = backSplit = NULL;   

            if (currElem != NULL)
            {
               const FastFloat objPlaneClassification = currElem->elem->getBoundingVolume().distanceToPlane( *planes[planeIdx] );

               if ( objPlaneClassification < Float_0 )
               {
                  backSplit = currElem;
               }
               else if ( objPlaneClassification == Float_0 )
               {
                  // object intersects the plane
                  Elem* frontElem = NULL;
                  Elem* backElem = NULL;
                  currElem->elem->split( *planes[planeIdx], &frontElem, &backElem );

                  SGHandle oldHandle = currElem->handle;
                  delete currElem;

                  if (frontElem != NULL) 
                  {
                     frontSplit = new SGElement(oldHandle, frontElem);
                  }
                  if (backElem != NULL) 
                  {
                     backSplit = new SGElement(oldHandle, backElem);
                  }
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

      // determine where the elements from the final split will be located
      unsigned int elemOffsetShift = 0;
      switch ( splitPlanesCount )
      {
      case 1: elemOffsetShift = 1; break;
      case 2: elemOffsetShift = 3; break;
      case 3: elemOffsetShift = 7; break;
      default: ASSERT_MSG(false, "invalid number of split planes");
      };
      for ( unsigned int subSectorIdx = 0; subSectorIdx < childrenCount; ++subSectorIdx )
      {
         SGElement* elem = splitElems[subSectorIdx + elemOffsetShift];
         if (elem != NULL) 
         {
            addElemToTree(elem, subTreeRoot.getChild(subSectorIdx), changedSectors);
         }
      }
   }
   else
   {
      // this is a leaf - add the element to it
      unsigned int elemIdx = addElement(element);

      subTreeRoot.m_elems.push_back(elemIdx);
      m_handles[element->handle]->push_back(elemIdx);
      changedSectors.push_back(&subTreeRoot);
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void StaticGeometryOctree<Elem>::remove(SGHandle elemHandle)
{
   Array< Sector*, MemoryPoolAllocator > sectors( 16, m_allocator );

   SGElementParts& elemParts = *(m_handles[elemHandle]);
   unsigned int count = elemParts.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      unsigned int elemIdx = elemParts[i];

      // remove the element from the tree sectors
      sectors.clear();
      querySectors( m_elements[elemIdx]->elem->getBoundingVolume(), *m_root, sectors );
      unsigned int sectorsCount = sectors.size();
      unsigned int idx;
      for ( unsigned int j = 0; j < sectorsCount; ++j )
      {
         idx = sectors[j]->m_elems.find( elemIdx );
         if ( idx == EOA ) 
         {
            continue;
         }
         else 
         {
            sectors[j]->m_elems.remove( idx );
         }
      }

      // delete the element
      delete releaseElement(elemIdx);
   }

   // release the handle to the element
   releaseHandle(elemHandle);

   recalculateElementsBounds();
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
unsigned int StaticGeometryOctree<Elem>::getElementsCount() const
{
   return m_elements.size();
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Elem& StaticGeometryOctree<Elem>::getElement(unsigned int idx) const
{
   return *(m_elements[idx]->elem);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
unsigned int StaticGeometryOctree<Elem>::addElement(SGElement* element)
{
   return m_elements.insert(element);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
typename StaticGeometryOctree<Elem>::SGElement* 
StaticGeometryOctree<Elem>::releaseElement(unsigned int idx)
{
   SGElement* element = m_elements[idx];
   m_elements[idx] = NULL;
   m_elements.remove(idx);

   return element;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
SGHandle StaticGeometryOctree<Elem>::addHandle()
{
   SGHandle handle = m_handles.insert(new SGElementParts());
   return handle;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void StaticGeometryOctree<Elem>::releaseHandle(SGHandle idx)
{
   delete m_handles[idx];
   m_handles.remove(idx);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
unsigned int StaticGeometryOctree<Elem>::getSplitElementsCount() const
{
   return 0;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _STATIC_GEOMETRY_OCTREE_H
