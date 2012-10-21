/// @file   core\SharedSpatialStorage.h
/// @brief  spatial storage decorator that doesn't destroy the decorated storage
#ifndef _SHARED_SPATIAL_STORAGE_H
#define _SHARED_SPATIAL_STORAGE_H

#include "core\SpatialStorage.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * Spatial storage decorator that doesn't destroy the decorated storage.
 */
template <typename Elem>
class SharedSpatialStorage : public SpatialStorage<Elem>
{
   DECLARE_ALLOCATOR( SharedSpatialStorage, AM_DEFAULT );

private:
   SpatialStorage<Elem>& m_storage;

public:
   /**
    * Constructor.
    *
    * @param storage    wrapped storage
    */
   SharedSpatialStorage(SpatialStorage<Elem>& storage);

   // -------------------------------------------------------------------------
   // SpatialStorage implementation
   // -------------------------------------------------------------------------
   void query(const BoundingVolume& boundingVol, Array<Elem*>& output) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\SharedSpatialStorage.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _SHARED_SPATIAL_STORAGE_H
