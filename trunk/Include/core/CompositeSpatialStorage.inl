#ifndef _COMPOSITE_SPATIAL_STORAGE_H
#error "This file can only be included from CompositeSpatialStorage.h"
#else

#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template <typename Elem>
CompositeSpatialStorage<Elem>::~CompositeSpatialStorage()
{
   unsigned int count = m_storages.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_storages[i];
   }
   m_storages.clear();
}

///////////////////////////////////////////////////////////////////////////////

template <typename Elem>
void CompositeSpatialStorage<Elem>::add(SpatialStorage<Elem>* storage)
{
   if (storage == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a SpatialStorage instance");
   }

   m_storages.push_back(storage);
}

///////////////////////////////////////////////////////////////////////////////

template <typename Elem>
void CompositeSpatialStorage<Elem>::query(const BoundingVolume& boundingVol, 
                                          Array<Elem*>& output) const
{
   unsigned int count = m_storages.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_storages[i]->query(boundingVol, output);
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _COMPOSITE_SPATIAL_STORAGE_H
