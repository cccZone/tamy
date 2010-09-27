#pragma once

/// @file   core-Renderer\Geometry.h
/// @brief  3D geometry representation

#include "core-MVC\Entity.h"


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;
class GeometryResource;
class SpatialEntity;
class Camera;

///////////////////////////////////////////////////////////////////////////////

/**
 * A common interface for 3D geometry which can be rendered.
 */
class Geometry : public Entity
{
   DECLARE_CLASS( Geometry )

protected:
   // runtime data
   Camera*                 m_camera;
   SpatialEntity*          m_parentNode;

private:
   // static data
   GeometryResource*       m_resource;

   // runtime data
   bool                    m_visible;

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
   Geometry( GeometryResource& resource );
   ~Geometry();

   /**
    * Renders the geometry.
    */
   void render();

   /**
    * Calculates a bounding volume around the geometry ( in the parent's space,
    * if the entity is attached to one ).
    *
    * @param   bounding volume instance
    */
   BoundingVolume* calculateBoundingVolume() const;

   /**
    * Returns the name of the geometry resource used by this geometry entity.
    */
   std::string getGeometryName() const;

   /**
    * Tells if the entity has a geometry resource attached.
    */
   inline bool hasGeometry() const { return m_resource != NULL; }

   /**
    * Returns the attached geometry resource.
    */
   inline GeometryResource& getGeometry() const { return *m_resource; }

   /**
    * Changes visibility status of the renderable.
    *
    * @param visible    new visibility status
    */
   inline void setVisible( bool visible ) { m_visible = visible; }

   /**
    * Returns current visibility status of the entity
    */
   inline bool isVisible() const { return m_visible; }

protected:
   /**
    * Called just before the geometry is rendered.
    */
   virtual void onPreRender() {}

   /**
    * Called right after the geometry is rendered.
    */
   virtual void onPostRender() {}

   // -------------------------------------------------------------------------
   // Object implementation 
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onAttached( Model& hostModel );
   void onDetached( Model& hostModel );
   void onAttached( Entity& parent );
   void onDetached( Entity& parent );
   void onComponentAdded( Component< Model >& component );
};

///////////////////////////////////////////////////////////////////////////////
