#pragma once

/// @file   core-Scene\SpatiallyQueryable.h
/// @brief  marker interface for things that can be fully queried from spatial
///         storages.

#include "core\Array.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;
struct Triangle;
class Entity;

///////////////////////////////////////////////////////////////////////////////

/**
 * Something is spatially queryable when we can perform a broad- 
 * and narrow phase query on it.
 */
class SpatiallyQueryable
{
public:
   virtual ~SpatiallyQueryable() {}

   /**
    * This method returns the global bounding volume that bounds 
    * the node's contents.
    *
    * @return  silhouette's bounding volume
    */
   virtual const BoundingVolume& getBoundingVolume() = 0;

   /**
    * This method returns the detailed geometry that bounds the
    * contents of a node. 
    *
    * @return  triangles the silhouette is comprised of
    */
   virtual const Array<Triangle*>& getBoundingGeometry() = 0;

   /**
    * Returns a matrix with the global situation of the queryable.
    *
    * @return  queryable world situation
    */
   virtual const D3DXMATRIX& getGlobalMtx() = 0;

   /**
    * Gives access to the encapsulated entity.
    *
    * @return  entity
    */
   virtual Entity& getEntity() = 0;

};

///////////////////////////////////////////////////////////////////////////////
