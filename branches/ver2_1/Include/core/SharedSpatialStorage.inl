#ifndef _SHARED_SPATIAL_STORAGE_H
#error "This file can only be included from SharedSpatialStorage.h"
#else


///////////////////////////////////////////////////////////////////////////////

template <typename Elem>
SharedSpatialStorage<Elem>::SharedSpatialStorage(SpatialStorage<Elem>& storage)
: m_storage(storage)
{
}

///////////////////////////////////////////////////////////////////////////////

template <typename Elem>
void SharedSpatialStorage<Elem>::query(const BoundingVolume& boundingVol, 
                                       Array<Elem*>& output) const
{
   m_storage.query(boundingVol, output);
}

///////////////////////////////////////////////////////////////////////////////

#endif // _SHARED_SPATIAL_STORAGE_H
