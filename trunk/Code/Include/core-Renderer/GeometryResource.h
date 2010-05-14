#pragma once

/// @file   core-Renderer\GeometryResource.h
/// @brief  resource with geometry data

#include "core\Resource.h"


class BoundingVolume;

///////////////////////////////////////////////////////////////////////////////

/**
 * A resource for creating geometry data.
 */
class GeometryResource : public Resource
{
   DECLARE_CLASS( GeometryResource )

public:
   /**
    * Constructor.
    */
   GeometryResource( const std::string& name = "" ) : Resource( name ) {}

   virtual ~GeometryResource() {}

   /**
    * Renders the geometry.
    */
   virtual void render() {}

   /**
    * Calculates a bounding volume around the geometry.
    *
    * @param   new bounding volume instance
    */
   virtual BoundingVolume* calculateBoundingVolume() const { return NULL;}
};

///////////////////////////////////////////////////////////////////////////////
