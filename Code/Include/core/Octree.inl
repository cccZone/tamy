#ifndef _OCTREE_H
#error "This file can only be included from Octree.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Octree<Elem>::Octree(const AABoundingBox& treeBB)
: m_root(new Sector(treeBB))
{
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Octree<Elem>::~Octree()
{
   delete m_root;
   m_root = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::query(const BoundingVolume& boundingVol, Array<Elem*>& output) const
{
   Array<Sector*> candidateSectors;
   querySectors(boundingVol, *m_root, candidateSectors);

   unsigned int elemsCount = getElementsCount();
   Array<bool> elemsToOutput;
   elemsToOutput.resize(elemsCount, false);

   unsigned int sectorsCount = candidateSectors.size();
   Array<unsigned int>* elemsList = NULL;
   unsigned int sectorElemsCount = 0;
   for (unsigned int i = 0; i < sectorsCount; ++i)
   {
      elemsList = (&candidateSectors[i]->m_elems);
      sectorElemsCount = elemsList->size();
      for (unsigned int j = 0; j < sectorElemsCount; ++j)
      {
         elemsToOutput[(*elemsList)[j]] = true;
      }
   }

   // add each element only once - providing it's inside the query volume
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      if (elemsToOutput[i] == false) continue;
      
      Elem& elem = getElement(i);
      if (elem.getBoundingVolume().testCollision(boundingVol))
      {
         output.push_back(&elem);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::querySectors(const BoundingVolume& boundingVol, 
                  Sector& searchRoot,
                  Array<Sector*>& output) const
{
   Stack<Sector*> stack;
   stack.push(&searchRoot);

   while(stack.empty() == false)
   {
      Sector* currSector = stack.pop();

      if (currSector->doesIntersect(boundingVol) == false) {continue;}

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
         output.push_back(currSector);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::subdivideTree(Sector& root, unsigned int depth)
{
   unsigned int desiredDepth = root.getDepth() + depth;

   std::list<Sector*> sectorsQueue;
   sectorsQueue.push_back(m_root);
   
   while (sectorsQueue.size())
   {
      Sector* sector = sectorsQueue.front();
      
      if (sector->getDepth() < desiredDepth)
      {
         sector->subdivide();

         unsigned int childrenCount = sector->getChildrenCount();
         for (unsigned int i = 0; i < childrenCount; ++i)
         {
            sectorsQueue.push_back(&sector->getChild(i));
         }
      }

      sectorsQueue.pop_front();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::clearSectors()
{
   AABoundingBox treeBB = m_root->getBoundingBox();
   unsigned int depth = m_root->getDepth();
   delete m_root;
   m_root = new Sector( treeBB, depth );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _OCTREE_H
