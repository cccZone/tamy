/// @file   TamyEditor/RPBDebugRender.h
/// @brief  rendering pipeline block representing a node that renders a debug scenes
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPDebugRendererNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a node that renders scenes.
 */
class RPBDebugRender : public RenderingPipelineBlock
{
   DECLARE_CLASS( RPBDebugRender )

private:
   RPDebugRendererNode*                               m_nodePtr;
   TResourceHandle< RPDebugRendererNode >*            m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBDebugRender() : m_nodePtr( NULL ), m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBDebugRender( RPDebugRendererNode& node );
   ~RPBDebugRender();

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   Object& getNode();
   void initialize( RenderingPipeline& parentResource );

protected:
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }
   virtual QColor getBgColor() const { return QColor( 166, 198, 224 ); }
};

///////////////////////////////////////////////////////////////////////////////
