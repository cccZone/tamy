#ifndef _DYNAMIC_OCTREE_H
#error "This file can only be included from DynamicOctree.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
DynamicOctree<Elem>::DynamicOctree(const AABoundingBox& treeBB, int treeDepth)
: Octree(treeBB)
, m_elements(65536)
{
   if (treeDepth < 0) {treeDepth = 0;}

   subdivideTree(*m_root, treeDepth);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
DynamicOctree<Elem>::~DynamicOctree()
{
   unsigned int elemsCount = m_elements.size();
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      delete m_elements[i];
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void DynamicOctree<Elem>::update(Elem& elem)
{  
   // remove the element from the sectors it's in
   TreeElem* treeElem = NULL;
   unsigned int elemsCount = m_elements.size();
   unsigned int elemIdx = 0;
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      if (&elem == m_elements[i]->elem) 
      {
         treeElem = m_elements[i];
         elemIdx = i;
         break;
      }
   }
   if (treeElem == NULL) {return;}

   unsigned int sectorsCount = treeElem->hostSectors.size();
   for (unsigned int i = 0; i < sectorsCount; ++i)
   {
      Array<unsigned int>& sectorElems = treeElem->hostSectors[i]->m_elems;
      unsigned int idx = sectorElems.find(elemIdx);
      if (idx != EOA) {sectorElems.remove(idx);}
   }

   putElemInTree(elemIdx, *treeElem);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void DynamicOctree<Elem>::clearTree()
{
   Stack<Sector*> sectors;
   sectors.push(m_root);

   while(sectors.empty() == false)
   {
      Sector* currSector = sectors.pop();

      currSector->m_elems.clear();
      if (currSector->m_children != NULL)
      {
         for (unsigned int i = 0; i < 8; ++i)
         {
            sectors.push(currSector->m_children[i]);
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
bool DynamicOctree<Elem>::isAdded(const Elem& elem) const
{
   unsigned int elemsCount = m_elements.size();
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      if (&elem == m_elements[i]->elem) {return true;}
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void DynamicOctree<Elem>::insert(Elem& elem)
{
   if (isAdded(elem) == true) {return;}

   // add the element to the collection of elements
   unsigned int elemIdx = m_elements.size();

   TreeElem* newElem = new TreeElem(&elem);
   if (m_freePos.empty() == false)
   {
      m_elements[m_freePos.pop()] = newElem;
   }
   else
   {
      m_elements.push_back(newElem);
   }

   putElemInTree(elemIdx, *newElem);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void DynamicOctree<Elem>::putElemInTree(unsigned int elemIdx, 
                                        TreeElem& treeElem)
{
   // place the element id in the correct sector
   Array<Sector*> candidateSectors;
   unsigned int sectorsCount = 0;

   querySectors(treeElem.elem->getBoundingVolume(), *m_root, candidateSectors);
   sectorsCount = candidateSectors.size();

   ASSERT(sectorsCount > 0, "The world is too small to add this element");

   treeElem.hostSectors.clear();
   Sector* sector = NULL;
   for (unsigned int i = 0; i < sectorsCount; ++i)
   {
      sector = candidateSectors[i];
      sector->m_elems.push_back(elemIdx);
      treeElem.hostSectors.push_back(sector);
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
unsigned int DynamicOctree<Elem>::getElementsCount() const
{
   return m_elements.size();
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Elem& DynamicOctree<Elem>::getElement(unsigned int idx) const
{
   return *(m_elements[idx]->elem);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void DynamicOctree<Elem>::remove(Elem& elem)
{
   TreeElem* treeElem = NULL;
   unsigned int removedElemIdx = 0;
   unsigned int elemsCount = m_elements.size();
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      if (&elem == m_elements[i]->elem) 
      {
         treeElem = m_elements[i];
         m_elements[i] = NULL;
         removedElemIdx = i;
         m_freePos.push(i);
         break;
      }
   }
   if (treeElem == NULL) {return;}

   unsigned int sectorsCount = treeElem->hostSectors.size();
   for (unsigned int i = 0; i < sectorsCount; ++i)
   {
      Array<unsigned int>& sectorElems = treeElem->hostSectors[i]->m_elems;
      unsigned int idx = sectorElems.find(removedElemIdx);
      if (idx != EOA) {sectorElems.remove(idx);}
   }
   delete treeElem;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
DynamicOctree<Elem>::TreeElem::TreeElem(Elem* _elem)
: elem(_elem)
{
}

///////////////////////////////////////////////////////////////////////////////

#endif // _DYNAMIC_OCTREE_H
