#ifndef _REGULAR_OCTREE_H
#error "This file can only be included from RegularOctree.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
RegularOctree<Elem>::RegularOctree(const AABoundingBox& treeBB, 
                                   unsigned int maxElemsPerSector,
                                   unsigned int maxTreeDepth,
                                   unsigned int initDepth)
: Octree(treeBB)
, m_elements(65536)
, m_maxElemsPerSector(maxElemsPerSector)
, m_maxTreeDepth(maxTreeDepth)
{
   if (m_maxElemsPerSector == 0) {m_maxElemsPerSector = 1;}

   if (initDepth < 0) {initDepth = 0;}
   if (initDepth > m_maxTreeDepth) {initDepth = m_maxTreeDepth;}
   subdivideTree(*m_root, initDepth);
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
   for (ElementsArray::iterator it = m_elements.begin(); 
        it != m_elements.end(); ++it)
   {
      if (&elem == (*it)) {return true;}
   }
   return false;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void RegularOctree<Elem>::insert(Elem& elem)
{
   if (isAdded(elem) == true) {return;}

   // insert the element to the collection of elements
   unsigned int elemIdx = m_elements.insert(&elem);

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
         (sector->getDepth() < m_maxTreeDepth))
      {
         sector->subdivide();
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
   m_elements.remove(removedElemIdx);

   // step through the tree and remove all the references to this element
   Stack<Sector*> stack;
   stack.push(m_root);

   while(stack.empty() == false)
   {
      Sector* currSector = stack.pop();

      unsigned int childrenCount = currSector->getChildrenCount();
      if (childrenCount > 0)
      {
         ASSERT(currSector->m_elems.size() == 0, "Composite node has an element assigned");
         for (unsigned int i = 0; i < childrenCount; ++i)
         {
            stack.push(&currSector->getChild(i));
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

template<typename Elem>
void RegularOctree<Elem>::clear()
{
   m_elements.clear();
   clearSectors();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _REGULAR_OCTREE_H
