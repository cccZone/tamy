#pragma once

/// @file   core-Renderer\GeometryResource.h
/// @brief  resource with geometry data

#include "core\Resource.h"


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;
class Renderer;

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
    * Calculates a bounding volume around the geometry.
    *
    * @param   new bounding volume instance
    */
   virtual const BoundingVolume& getBoundingVolume() const { return *( reinterpret_cast< BoundingVolume* >( NULL ) ); }

   /**
    * Renders the resource.
    *
    * @param renderer
    */
   virtual void render( Renderer& renderer ) {}
};

///////////////////////////////////////////////////////////////////////////////
