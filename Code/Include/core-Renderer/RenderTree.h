/// @file   core-Renderer/RenderTree.h
/// @brief  render tree definition
#pragma once

#include "core/MemoryPool.h"


///////////////////////////////////////////////////////////////////////////////

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
   StateTreeNode*          m_child;
   StateTreeNode*          m_sibling;

   RenderState*            m_state;
   MemoryPoolObject*       m_managedObj;
   GeometryNode*           m_geometryNode;

   /**
    * Constructor.
    */
   StateTreeNode();

   /**
    * Constructor.
    *
    * @param state         described render state
    */
   StateTreeNode( RenderState& state );

   /**
    * Constructor ( that will delete the state once it gets deleted ).
    *
    * @param state
    * @param managedObj    additional obj to destroy
    */
   StateTreeNode( RenderState* state, MemoryPoolObject* managedObj );
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
    * @param runtime data buffer
    */
   void render( Renderer& renderer ) const;
};

///////////////////////////////////////////////////////////////////////////////
