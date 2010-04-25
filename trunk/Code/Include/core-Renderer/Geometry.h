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

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onSaveEntity(Serializer& serializer) {}
   void onLoadEntity(Serializer& serializer) {}
   void onChildAttached(Entity& child) {}
   void onChildDetached(Entity& child) {}
   void onAttached(Entity& parent) {}
   void onDetached(Entity& parent) {}
   void onAttached(Model& hostModel) {}
   void onDetached(Model& hostModel) {}
};

///////////////////////////////////////////////////////////////////////////////
