/// @file   TamyEditor/GraphLayout.h
/// @brief  graph layout interface
#pragma once

#include <vector>
#include <QPointF>
#include "GraphBlock.h"
#include <QGraphicsScene>


///////////////////////////////////////////////////////////////////////////////

/**
 * An abstract graph layout interface. It stores a list of blocks, however those
 * are managed by a solid class implementing this interface.
 * The class should be a resource that will store the layout data and provide
 * a link with a model resource the blocks represent.
 */
class GraphLayout : public QGraphicsScene
{
protected:
   // the blocks will be managed by an outside resource
   std::vector< GraphBlock* >       m_blocks;

public:
   virtual ~GraphLayout();

   /**
    * Adds a new block to the layout.
    *
    * @param block
    */
   GraphBlock& add( const Class& type );

   /**
    * Removes a block from the layout.
    *
    * @param block
    */
   void remove( GraphBlock* block );

   /**
    * Returns a list of types we can select from when creating a new node.
    *
    * @param classes          fill this list with available node types
    */
   virtual void getNodesClasses( std::vector< Class >& classes ) = 0;
   
protected:
   /**
    * Default constructor.
    *
    * @param path
    */
   GraphLayout( const std::string& path = "" );

   /**
    * Caches the layout's state for storing purposes.
    */
   void saveState();

   /**
    * Restores the cached layout state after the load.
    */
   void restoreState();

   // -------------------------------------------------------------------------
   // Representations factory
   // -------------------------------------------------------------------------

   /**
    * Called when the widget creates a new representation for the specified node type.
    *
    * @param type    node type
    */
   virtual GraphBlock* createNode( const Class& type ) = 0;

   /**
    * Called when the widget removes a node.
    *
    * @param node
    */
   virtual void removeNode( Object& node ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
