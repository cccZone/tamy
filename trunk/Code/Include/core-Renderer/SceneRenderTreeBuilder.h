/// @file   core-Renderer/SceneRenderTreeBuilder.h
/// @brief  a strategy for building a scene tree for rendering
#pragma once

#include "core/Array.h"
#include "core/MemoryPool.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialRepresentation;
class Geometry;
class RenderState;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A geometry node in the scene render tree - this is the tree's leaf.
 */
struct GeometryNode : public MemoryPoolObject
{
   GeometryNode*  m_next;
   Geometry&      m_geometry;

   /**
    * Constructor.
    *
    * @param nextNode      the node can have siblings, but only ones containing geometry rendered with the same render state
    * @param geometry      rendered geometry
    */
   GeometryNode( GeometryNode*& nextNode, Geometry& geometry );
   ~GeometryNode();

   /**
    * Renders the node's contents.
    *
    * @param renderer
    */
   void render( Renderer& renderer ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A state node in the scene render tree - this is the tree's composite node.
 */
struct StateTreeNode : public MemoryPoolObject
{
   StateTreeNode*    m_child;
   StateTreeNode*    m_sibling;

   RenderState*      m_state;
   GeometryNode*     m_geometryNode;

   /**
    * Constructor.
    *
    * @param state         described render state
    */
   StateTreeNode( RenderState* state );
   ~StateTreeNode();

   /**
    * Compares another render state with the one contained in this node.
    *
    * @param state      other render state
    */ 
   bool compareState( RenderState* state );

   /**
    * Sets the render state for rendering.
    *
    * @param renderer
    */
   void render( Renderer& renderer ) const;
};

///////////////////////////////////////////////////////////////////////////////


/**
 * A strategy for building a scene tree for rendering.
 */
class SceneRenderTreeBuilder
{
public:
   virtual ~SceneRenderTreeBuilder() {}

   /**
    * Build the tree here.
    *
    * @param pool             memory pool in which we can allocate the tree nodes
    * @param visibleElems     array of visible elements we want to render.
    */
   virtual StateTreeNode* buildRenderTree( MemoryPool& pool, const Array< SpatialRepresentation* >& visibleElems ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
