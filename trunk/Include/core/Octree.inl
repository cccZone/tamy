#ifndef _OCTREE_H
#error "This file can only be included from Octree.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Octree<Elem>::Octree(const AABoundingBox& treeBB)
: m_root(new Sector())
{
   m_root->m_bb = treeBB;
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
void Octree<Elem>::subdivideSector(Sector& sector)
{
   ASSERT(sector.m_children == NULL, "This already is a composite sector");

   // create child sectors
   typedef Sector* SectorP;
   sector.m_children = new SectorP[8];
   for (int i = 0; i < 8; ++i)
   {
      sector.m_children[i] = new Sector();
      sector.m_children[i]->m_depth = sector.m_depth + 1;
   }
   AABoundingBox* bb = NULL;

   D3DXVECTOR3 midPoint = (sector.m_bb.max + sector.m_bb.min) / 2.f;

   bb = &(sector.m_children[CS_FRONT_LEFT_UPPER]->m_bb);
   *bb = sector.m_bb;
   bb->max.x = midPoint.x;
   bb->min.y = midPoint.y;
   bb->min.z = midPoint.z;

   bb = &(sector.m_children[CS_FRONT_RIGHT_UPPER]->m_bb);
   *bb = sector.m_bb;
   bb->min.x = midPoint.x;
   bb->min.y = midPoint.y;
   bb->min.z = midPoint.z;

   bb = &(sector.m_children[CS_FRONT_RIGHT_LOWER]->m_bb);
   *bb = sector.m_bb;
   bb->min.x = midPoint.x;
   bb->max.y = midPoint.y;
   bb->min.z = midPoint.z;

   bb = &(sector.m_children[CS_FRONT_LEFT_LOWER]->m_bb);
   *bb = sector.m_bb;
   bb->max.x = midPoint.x;
   bb->max.y = midPoint.y;
   bb->min.z = midPoint.z;

   bb = &(sector.m_children[CS_BACK_LEFT_UPPER]->m_bb);
   *bb = sector.m_bb;
   bb->max.x = midPoint.x;
   bb->min.y = midPoint.y;
   bb->max.z = midPoint.z;

   bb = &(sector.m_children[CS_BACK_RIGHT_UPPER]->m_bb);
   *bb = sector.m_bb;
   bb->min.x = midPoint.x;
   bb->min.y = midPoint.y;
   bb->max.z = midPoint.z;

   bb = &(sector.m_children[CS_BACK_RIGHT_LOWER]->m_bb);
   *bb = sector.m_bb;
   bb->min.x = midPoint.x;
   bb->max.y = midPoint.y;
   bb->max.z = midPoint.z;

   bb = &(sector.m_children[CS_BACK_LEFT_LOWER]->m_bb);
   *bb = sector.m_bb;
   bb->max.x = midPoint.x;
   bb->max.y = midPoint.y;
   bb->max.z = midPoint.z;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::subdivideTree(Sector& root, unsigned int depth)
{
   unsigned int desiredDepth = root.m_depth + depth;

   std::list<Sector*> sectorsQueue;
   sectorsQueue.push_back(m_root);
   
   while (sectorsQueue.size())
   {
      Sector* sector = sectorsQueue.front();
      
      if (sector->m_depth < desiredDepth)
      {
         subdivideSector(*sector);

         for (unsigned int i = 0; i < 8; ++i)
         {
            sectorsQueue.push_back(sector->m_children[i]);
         }
      }

      sectorsQueue.pop_front();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Octree<Elem>::Sector::Sector()
: m_depth(0)
, m_children(NULL)
{}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Octree<Elem>::Sector::~Sector()
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

#endif // _OCTREE_H
