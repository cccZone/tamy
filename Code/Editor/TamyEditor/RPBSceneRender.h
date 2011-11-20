/// @file   TamyEditor/RPBSceneRender.h
/// @brief  rendering pipeline block representing a node that renders scenes
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPSceneRenderNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a node that renders scenes.
 */
class RPBSceneRender : public TRenderingPipelineBlock< RPSceneRenderNode >
{
   DECLARE_CLASS( RPBSceneRender )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBSceneRender() : TRenderingPipelineBlock< RPSceneRenderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBSceneRender( RPSceneRenderNode& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }
   virtual QColor getBgColor() const { return QColor( 166, 198, 224 ); }
};

///////////////////////////////////////////////////////////////////////////////
