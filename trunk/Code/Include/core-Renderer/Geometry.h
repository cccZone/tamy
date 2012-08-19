/// @file   core-Renderer\Geometry.h
/// @brief  3D geometry representation
#pragma once

#include "core-MVC\SpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;
class GeometryResource;
class RenderState;
class Renderer;
class VertexShaderConfigurator;
class RCBindVertexShader;
struct AABoundingBox;

///////////////////////////////////////////////////////////////////////////////

typedef std::vector< RenderState* >    RenderStatesVec;

///////////////////////////////////////////////////////////////////////////////

/**
 * A common interface for 3D geometry which can be rendered.
 */
class Geometry : public SpatialEntity
{
   DECLARE_CLASS()

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
    *
    * @param name
    */
   Geometry( const std::string& name = "" );

   /**
    * Copy constructor.
    */
   Geometry( const Geometry& rhs );

   /**
    * Constructor.
    *
    * @param resource   shared geometry resource
    * @param name
    */
   Geometry( GeometryResource& resource, const std::string& name = "" );
   ~Geometry();

   /**
    * Renders the geometry.
    * 
    * @param renderer
    * @param externalConfigurator
    */
   virtual void render( Renderer& renderer, VertexShaderConfigurator* externalConfigurator = NULL );

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
   virtual const BoundingVolume& getBoundingVolume() const;

   /**
    * Calculates a bounding box around the geometry ( in world space coordinates ).
    *
    * @param boundingBox
    */
   void getBoundingBox( AABoundingBox& boundingBox ) const;

   /**
    * Sets the new geometry resource.
    *
    * @param 
    */
   void setMesh( GeometryResource& mesh );

   /**
    * Returns the name of the geometry resource used by this geometry entity.
    */
   std::string getMeshName() const;

   /**
    * Tells if the entity has a geometry resource attached.
    */
   inline bool hasMesh() const { return m_resource != NULL; }

   /**
    * Returns the attached geometry resource.
    */
   inline GeometryResource* getMesh() const { return m_resource; }

protected:
   /**
    * Called before the geometry rendering command is issued.
    *
    * @param renderer
    * @return render command that binds and configures a vertex shader used to render this piece of geometry ( or NULL if the geometry should not be rendered )
    */
   virtual RCBindVertexShader* onPreRender( Renderer& renderer ) { return NULL; }

   /**
    * Called after the geometry rendering command is issued.
    *
    * @param renderer
    * @param externalConfigurator
    */
   virtual void onPostRender( Renderer& renderer ) {}

   // -------------------------------------------------------------------------
   // Object implementation 
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( ReflectionProperty& property );

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onChildAttached( Entity& child );
   void onChildDetached( Entity& child );
   Entity* cloneSelf() const; 
};

///////////////////////////////////////////////////////////////////////////////
