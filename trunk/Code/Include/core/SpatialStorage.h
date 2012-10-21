/// @file   core\SpatialStorage.h
/// @brief  a spatial storage interface
#pragma once

#include "core\MemoryRouter.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;

///////////////////////////////////////////////////////////////////////////////

/**
 * A spatial storage stores elements that have a position in space and a volume
 * in an efficient manner, optimized for query time.
 *
 * An example of a spatial storage would be an octree, a quad tree etc.
 */
template<typename Elem>
class SpatialStorage
{
   DECLARE_ALLOCATOR( SpatialStorage< Elem >, AM_ALIGNED_16 );

public:
   virtual ~SpatialStorage() {}

   /**
    * This method allows to query all the elements that are contained
    * in the passed bounding volume.
    *
    * @param boundingVol   volume that bounds the elements we want
    * @param output        upon method return this array will be filled with
    *                      elements overlapping the query volume
    */
   virtual void query( const BoundingVolume& boundingVol, Array<Elem*>& output ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
