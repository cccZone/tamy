/// @file   core\NarrowPhaseStorageFilter.h
/// @brief  spatial storage decorator with narrow-phase query filter
#ifndef _NARROW_PHASE_STORAGE_FILTER_H
#define _NARROW_PHASE_STORAGE_FILTER_H

#include "core\SpatialStorage.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This filter will provide additional level of checking if the queried
 * elements fit the bounding volume by testing the volume
 * not only against the bounding volumes of the elements, but also
 * against their silhouettes made up of triangles.
 */
template <typename Elem>
class NarrowPhaseStorageFilter : public SpatialStorage<Elem>
{
   ALIGNED_CLASS();

private:
   SpatialStorage<Elem>* m_storage;

public:
   /**
    * Constructor.
    *
    * @param storage    storage query to which we want to narrow-phase filter
    */
   NarrowPhaseStorageFilter(SpatialStorage<Elem>* storage);
   ~NarrowPhaseStorageFilter();

   // -------------------------------------------------------------------------
   // SpatialStorage implementation
   // -------------------------------------------------------------------------
   void query(const BoundingVolume& boundingVol, Array<Elem*>& output) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\NarrowPhaseStorageFilter.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _NARROW_PHASE_STORAGE_FILTER_H
