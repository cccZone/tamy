/// @file   core\AdaptingSpatialStorage.h
/// @brief  spatial storage that adapts the type of results it returns
#ifndef _ADAPTING_SPATIAL_STORAGE_H
#define _ADAPTING_SPATIAL_STORAGE_H

#include "core\SpatialStorage.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * Spatial storage that adapts the type of results it returns.
 */
template <typename FromType, typename ToType>
class AdaptingSpatialStorage : public SpatialStorage<ToType>
{
   DECLARE_ALLOCATOR( AdaptingSpatialStorage, AM_DEFAULT );

private:
   SpatialStorage<FromType>* m_storage;

public:
   /**
    * Constructor.
    *
    * @param storage    storage results of which we want to adapt
    */
   AdaptingSpatialStorage(SpatialStorage<FromType>* storage);
   ~AdaptingSpatialStorage();

   // -------------------------------------------------------------------------
   // SpatialStorage implementation
   // -------------------------------------------------------------------------
   void query(const BoundingVolume& boundingVol, Array<ToType*>& output) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\AdaptingSpatialStorage.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _ADAPTING_SPATIAL_STORAGE_H
