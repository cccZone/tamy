#pragma once

/// @file   core-Scene\Spatial.h
/// @brief  marker interface for all entities with volume

#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;
struct Triangle;

///////////////////////////////////////////////////////////////////////////////

class Spatial
{
public:
   virtual ~Spatial() {}

   /**
    * This method returns a primitive bounding volume of the entire 
    * entity's silhouette (in world space coordinates)
    *
    * @return  entity primitive bounding volume
    */
   virtual const BoundingVolume& getBoundingVolume() = 0;

   /**
    * The method returns the geometry of the entity's silhouette (in model
    * space).
    *
    * @return  triangles the entity's geometry is comprised of.
    */
   virtual const Array<Triangle*>& getBoundingGeometry() = 0;
};

///////////////////////////////////////////////////////////////////////////////
