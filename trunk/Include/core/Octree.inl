#ifndef _OCTREE_H
#error "This file can only be included from Octree.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Octree<Elem>::Octree(unsigned int maxElemsPerSector, float worldSize)
: m_elements(65536)
, m_maxElemsPerSector(maxElemsPerSector)
, m_root(new Sector())
{
   m_root->m_bb.min.x = -worldSize;
   m_root->m_bb.min.y = -worldSize;
   m_root->m_bb.min.z = -worldSize;
   m_root->m_bb.max.x = worldSize;
   m_root->m_bb.max.y = worldSize;
   m_root->m_bb.max.z = worldSize;

   if (m_maxElemsPerSector == 0) {m_maxElemsPerSector = 1;}
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
bool Octree<Elem>::isAdded(const Elem& elem) const
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
void Octree<Elem>::insert(Elem& elem)
{
   if (isAdded(elem) == true) {return;}

   // add the element to the collection of elements
   unsigned int elemIdx = m_elements.size();
   m_elements.push_back(&elem);

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

      if (sector->m_elems.size() > m_maxElemsPerSector)
      {
         subdivideSector(*sector);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::remove(Elem& elem)
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
         unsigned int elemsCount = currSector->m_elems.size();
         for (unsigned int i = 0; i < elemsCount; )
         {
            if (currSector->m_elems[i] == removedElemIdx)
            {
               currSector->m_elems.remove(i);
               --elemsCount;
            }
            else if (currSector->m_elems[i] > removedElemIdx)
            {
               --(currSector->m_elems[i]);
               ++i;
            }
            else
            {
               ++i;
            }
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::query(const BoundingVolume& boundingVol, Array<Elem*>& output) const
{
   Array<Sector*> candidateSectors;
   querySectors(boundingVol, *m_root, candidateSectors);

   unsigned int elemsCount = m_elements.size();
   Array<bool> elemsToOutput(elemsCount);
   for (unsigned int i = 0; i < elemsCount; ++i)
   {
      elemsToOutput.push_back(false);
   }

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

      if (m_elements[i]->getBoundingVolume().testCollision(boundingVol))
      {
         output.push_back(m_elements[i]);
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
Octree<Elem>::Sector::Sector()
: m_children(NULL)
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
