/// @file   TamyEditor/RPBBackBuffer.h
/// @brief  rendering pipeline block representing a back buffer render target node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPBackBufferNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a back buffer render target node.
 */
class RPBBackBuffer : public RenderingPipelineBlock
{
   DECLARE_CLASS( RPBBackBuffer )

private:
   RPBackBufferNode*                                 m_nodePtr;
   TResourceHandle< RPBackBufferNode >*              m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBBackBuffer() : m_nodePtr( NULL ), m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBBackBuffer( RPBackBufferNode& node );
   ~RPBBackBuffer();

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   Object& getNode();
   void initialize( RenderingPipeline& parentResource );

protected:
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_ROUNDED; }
   virtual QColor getBgColor() const { return QColor( 186, 166, 221 ); }
};

///////////////////////////////////////////////////////////////////////////////
