#pragma once

/// @file   core-Renderer\RenderingView.h
/// @brief  view that renders renderable entities

#include <map>
#include <vector>
#include "core-MVC\ModelView.h"
#include "core/RegularOctree.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialRepresentation;
class Renderer;
class Geometry;
struct AABoundingBox;
class Camera;
class RenderState;

///////////////////////////////////////////////////////////////////////////////

/**
* This view manages the visibility of renderables.
*/ 
class RenderingView : public ModelView
{
private:
   typedef std::map< Geometry*, SpatialRepresentation* >    SpatialsMap;

private:
   Renderer&                                                m_renderer;

   SpatialsMap                                              m_spatials;
   RegularOctree< SpatialRepresentation >*                  m_storage;

public:
   /**
    * Constructor.
    */
   RenderingView( Renderer& renderer, const AABoundingBox& sceneBB );
   ~RenderingView();

   /**
    * Renders the view contents.
    */
   void render();

   // ----------------------------------------------------------------------
   // ModelView implementation
   // ----------------------------------------------------------------------
   void onEntityAdded( Entity& entity );
   void onEntityRemoved( Entity& entity );
   void onEntityChanged( Entity& entity );

protected:
   void resetContents();

private:
   // ----------------------------------------------------------------------
   // State tree
   // ----------------------------------------------------------------------
   struct GeometryNode
   {
      GeometryNode*  m_next;
      Geometry&      m_geometry;

      GeometryNode( GeometryNode*& nextNode, Geometry& geometry );
      ~GeometryNode();

      void render( Renderer& renderer ) const;
   };

   struct StateTreeNode
   {
      StateTreeNode*    m_child;
      StateTreeNode*    m_sibling;

      RenderState*      m_state;
      GeometryNode*     m_geometryNode;

      StateTreeNode( RenderState* state );
      ~StateTreeNode();

      bool compareState( RenderState* state );
      void render( Renderer& renderer ) const;
   };

   StateTreeNode* buildStateTree( const Array< SpatialRepresentation* >& visibleElems ) const;

};

///////////////////////////////////////////////////////////////////////////////
