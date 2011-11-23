#pragma once

/// @file   core-Renderer\Geometry.h
/// @brief  3D geometry representation

#include "core-MVC\SpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;
class GeometryResource;
class RenderState;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

typedef std::vector< RenderState* >    RenderStatesVec;

///////////////////////////////////////////////////////////////////////////////

/**
 * A common interface for 3D geometry which can be rendered.
 */
class Geometry : public SpatialEntity
{
   DECLARE_CLASS( Geometry )

protected:
   // runtime data
   RenderStatesVec                  m_states;
   BoundingVolume*                  m_globalBounds;

private:
   // static data
   GeometryResource*                m_resource;

public:
   /**
    * Default constructor.
    */
   Geometry();

   /**
    * Copy constructor.
    */
   Geometry( const Geometry& rhs );

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
   virtual void render( Renderer& renderer );

   // -------------------------------------------------------------------------
   // Render states management
   // -------------------------------------------------------------------------
   /**
    * Adds a new render state.
    *
    * @param state
    */
   void addState( RenderState& state );

   /**
    * Removes a render state.
    *
    * @param state
    */
   void removeState( RenderState& state );

   /**
    * Returns a vector of the render states that affect the geometry.
    */
   const RenderStatesVec& getRenderStates() { return m_states; }

   // -------------------------------------------------------------------------
   // Geometry resource management
   // -------------------------------------------------------------------------
   /**
    * Calculates a bounding volume around the geometry ( in the parent's space,
    * if the entity is attached to one ).
    *
    * @param   bounding volume instance
    */
   virtual const BoundingVolume& calculateBoundingVolume() const;

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

protected:
   /**
    * Called before the geometry rendering command is issued.
    */
   virtual void onPreRender( Renderer& renderer ) {}

   /**
    * Called after the geometry rendering command is issued.
    */
   virtual void onPostRender( Renderer& renderer ) {}

   // -------------------------------------------------------------------------
   // Object implementation 
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onChildAttached( Entity& child );
   void onChildDetached( Entity& child );
   Entity* cloneSelf() const; 
};

///////////////////////////////////////////////////////////////////////////////
