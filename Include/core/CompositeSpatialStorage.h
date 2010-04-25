#ifndef _COMPOSITE_SPATIAL_STORAGE_H
#define _COMPOSITE_SPATIAL_STORAGE_H

/// @file   core\CompositeSpatialStorage.h
/// @brief  spatial storage composite

#include "core\SpatialStorage.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a composite of spatial storages, allowing to treat multiple storages
 * as one during queries.
 */
template <typename Elem>
class CompositeSpatialStorage : public SpatialStorage<Elem>
{
private:
   std::vector<SpatialStorage<Elem>*> m_storages;

public:
   ~CompositeSpatialStorage();

   /**
    * This method adds a new storage to the composite.
    *
    * @param storage    storage we want composed
    */
   void add(SpatialStorage<Elem>* storage);

   // -------------------------------------------------------------------------
   // SpatialStorage implementation
   // -------------------------------------------------------------------------
   void query(const BoundingVolume& boundingVol, Array<Elem*>& output) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\CompositeSpatialStorage.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _COMPOSITE_SPATIAL_STORAGE_H
