#pragma once

/// @file   core-Renderer\GeometryResource.h
/// @brief  resource with geometry data

#include <core\Array.h>
#include "core\Resource.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\Geometry.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class SingletonsManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * A resource for creating geometry data.
 */
class GeometryResource : public Resource
{
public:
   virtual ~GeometryResource() {}

   inline Geometry* load() {return new Geometry(*this);}

   /**
    * Renders the geometry.
    */
   virtual void render() = 0;

   /**
    * Calculates a bounding volume around the geometry.
    *
    * @param   new bounding volume instance
    */
   virtual BoundingVolume* calculateBoundingVolume() const = 0;

};

///////////////////////////////////////////////////////////////////////////////
