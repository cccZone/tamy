#ifndef _ADAPTING_SPATIAL_STORAGE_H
#error "This file can only be included from AdaptingSpatialStorage.h"
#else

#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template <typename FromType, typename ToType>
AdaptingSpatialStorage<FromType, ToType>::AdaptingSpatialStorage(SpatialStorage<FromType>* storage)
: m_storage(storage)
{
   if (m_storage == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a SpatialStorage instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename FromType, typename ToType>
AdaptingSpatialStorage<FromType, ToType>::~AdaptingSpatialStorage()
{
   delete m_storage; m_storage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <typename FromType, typename ToType>
void AdaptingSpatialStorage<FromType, ToType>::query(const BoundingVolume& boundingVol, 
                                                     Array<ToType*>& output) const
{
   Array<FromType*> tmpOutput;
   m_storage->query(boundingVol, tmpOutput);

   unsigned int count = tmpOutput.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      FromType* elem = tmpOutput[i];
      ToType* castElem = dynamic_cast<ToType*> (elem);
      if (castElem != NULL)
      {
         output.push_back(castElem);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _ADAPTING_SPATIAL_STORAGE_H
