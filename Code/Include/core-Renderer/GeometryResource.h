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
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   GeometryResource( const FilePath& resourceName = FilePath() ) : Resource( resourceName ) {}
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
