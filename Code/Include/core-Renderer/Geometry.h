#pragma once

/// @file   core-Renderer\Geometry.h
/// @brief  3D geometry representation

#include "core-MVC\Entity.h"


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;
class GeometryResource;

///////////////////////////////////////////////////////////////////////////////

/**
 * A common interface for 3D geometry which can be rendered.
 */
class Geometry : public Entity
{
   DECLARE_CLASS(Geometry)

private:
   GeometryResource* m_resource;
   BoundingVolume* m_vol;

public:
   /**
    * Default constructor.
    */
   Geometry();

   /**
    * Constructor.
    *
    * @param resource   shared geometry resource
    */
   Geometry(GeometryResource& resource);
   ~Geometry();

   /**
    * Renders the geometry.
    */
   void render();

   /**
    * Calculates a bounding volume around the geometry.
    *
    * @param   bounding volume instance
    */
   const BoundingVolume& calculateBoundingVolume() const;

   /**
    * Returns the name of the geometry resource used by this geometry entity.
    */
   std::string getGeometryName() const;

protected:
   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
};

///////////////////////////////////////////////////////////////////////////////
