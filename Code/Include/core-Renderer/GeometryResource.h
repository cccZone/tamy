#pragma once

/// @file   core-Renderer\GeometryResource.h
/// @brief  resource with geometry data


class BoundingVolume;

///////////////////////////////////////////////////////////////////////////////

/**
 * A resource for creating geometry data.
 */
class GeometryResource
{
public:
   virtual ~GeometryResource() {}

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
