#ifndef _DYNAMIC_OCTREE_H
#error "This file can only be included from DynamicOctree.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
DynamicOctree<Elem>::DynamicOctree(const AABoundingBox& treeBB, int treeDepth)
: m_elements(65536)
, m_root(new Sector())
{
   m_root->m_bb = treeBB;
   if (treeDepth < 0) {treeDepth = 0;}

   // subdivide tree to the desired depth
   struct SubdivisionStruct
   {
      Sector* sector;
      int depth;

      SubdivisionStruct(Sector* _sector, int _depth) 
         : sector(_sector), depth(_depth)
      {}
   };

   std::list<SubdivisionStruct> sectorsQueue;
   sectorsQueue.push_back(SubdivisionStruct(m_root, 0));
   
   while (sectorsQueue.size())
   {
      SubdivisionStruct& data = sectorsQueue.front();
      
      if (data.depth < treeDepth)
      {
         subdivideSector(*(data.sector));

         for (unsigned int i = 0; i < 8; ++i)
         {
            sectorsQueue.push_back(SubdivisionStruct(data.sector->m_children[i], 
                                                     data.depth + 1));
         }
      }

      sectorsQueue.pop_front();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
DynamicOctree<Elem>::~DynamicOctree()
{
   delete m_root;
   m_root = NULL;

   unsigned int elemsCount = m_elements.size();
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      delete m_elements[i];
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void DynamicOctree<Elem>::update()
{
   // we're gonna regenerate each elem position from scratch,
   // so simply clear all sectors contents and reinitialize it anew
   clearTree();
   
   unsigned int elemsCount = m_elements.size();
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      putElemInTree(i, *(m_elements[i]));
   }
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

   m_elements.push_back(new TreeElem(&elem));
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
void DynamicOctree<Elem>::remove(Elem& elem)
{
   TreeElem* treeElem = NULL;
   unsigned int elemsCount = m_elements.size();
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      if (&elem == m_elements[i]->elem) 
      {
         treeElem = m_elements[i];
         m_elements.remove(i);
      }
   }
   update();
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void DynamicOctree<Elem>::query(const BoundingVolume& boundingVol, Array<Elem*>& output) const
{
   Array<Sector*> candidateSectors;
   querySectors(boundingVol, *m_root, candidateSectors);

   unsigned int elemsCount = m_elements.size();
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

      if (m_elements[i]->elem->getBoundingVolume().testCollision(boundingVol))
      {
         output.push_back(m_elements[i]->elem);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void DynamicOctree<Elem>::querySectors(const BoundingVolume& boundingVol, 
                  Sector& searchRoot,
                  Array<Sector*>& output) const
{
   Stack<Sector*> stack;
   stack.push(&searchRoot);

   while(stack.empty() == false)
   {
      Sector* currSector = stack.pop();

      if (currSector->m_bb.testCollision(boundingVol) == false) {continue;}

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
         output.push_back(currSector);
      }
   }

}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void DynamicOctree<Elem>::subdivideSector(Sector& sector)
{
   ASSERT(sector.m_children == NULL, "This already is a composite sector");

   // create child sectors
   typedef Sector* SectorP;
   sector.m_children = new SectorP[8];
   for (int i = 0; i < 8; ++i)
   {
      sector.m_children[i] = new Sector();
   }
   AABoundingBox* bb = NULL;

   D3DXVECTOR3 midPoint(sector.m_bb.min.x + ((sector.m_bb.max.x - sector.m_bb.min.x) / 2.f),
      sector.m_bb.min.y + ((sector.m_bb.max.y - sector.m_bb.min.y) / 2.f),
      sector.m_bb.min.z + ((sector.m_bb.max.z - sector.m_bb.min.z) / 2.f));

   bb = &(sector.m_children[0]->m_bb);
   *bb = sector.m_bb;
   bb->max.x = midPoint.x;
   bb->min.y = midPoint.y;
   bb->min.z = midPoint.z;

   bb = &(sector.m_children[1]->m_bb);
   *bb = sector.m_bb;
   bb->min.x = midPoint.x;
   bb->min.y = midPoint.y;
   bb->min.z = midPoint.z;

   bb = &(sector.m_children[2]->m_bb);
   *bb = sector.m_bb;
   bb->min.x = midPoint.x;
   bb->max.y = midPoint.y;
   bb->min.z = midPoint.z;

   bb = &(sector.m_children[3]->m_bb);
   *bb = sector.m_bb;
   bb->max.x = midPoint.x;
   bb->max.y = midPoint.y;
   bb->min.z = midPoint.z;

   bb = &(sector.m_children[4]->m_bb);
   *bb = sector.m_bb;
   bb->max.x = midPoint.x;
   bb->min.y = midPoint.y;
   bb->max.z = midPoint.z;

   bb = &(sector.m_children[5]->m_bb);
   *bb = sector.m_bb;
   bb->min.x = midPoint.x;
   bb->min.y = midPoint.y;
   bb->max.z = midPoint.z;

   bb = &(sector.m_children[6]->m_bb);
   *bb = sector.m_bb;
   bb->min.x = midPoint.x;
   bb->max.y = midPoint.y;
   bb->max.z = midPoint.z;

   bb = &(sector.m_children[7]->m_bb);
   *bb = sector.m_bb;
   bb->max.x = midPoint.x;
   bb->max.y = midPoint.y;
   bb->max.z = midPoint.z;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
DynamicOctree<Elem>::Sector::Sector()
: m_children(NULL)
{}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
DynamicOctree<Elem>::Sector::~Sector()
{
   if (m_children != NULL)
   {
      for (int i = 0; i < 8; ++i)
      {
         delete m_children[i];
      }
   }
   delete [] m_children;
   m_children = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
DynamicOctree<Elem>::TreeElem::TreeElem(Elem* _elem)
: elem(_elem)
{
}

///////////////////////////////////////////////////////////////////////////////

#endif // _DYNAMIC_OCTREE_H
