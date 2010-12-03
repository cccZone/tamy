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
   void add( GraphBlock* block );

   /**
    * Removes a block from the layout.
    *
    * @param block
    */
   void remove( GraphBlock* block );
   
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
};

///////////////////////////////////////////////////////////////////////////////
