/// @file   TamyEditor/RPBCamera.h
/// @brief  rendering pipeline block representing a camera node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPCameraNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a camera node.
 */
class RPBCamera : public TRenderingPipelineBlock< RPCameraNode >
{
   DECLARE_CLASS( RPBCamera )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBCamera() : TRenderingPipelineBlock< RPCameraNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBCamera( RPCameraNode& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_ROUNDED; }
   virtual QColor getBgColor() const { return QColor( 248, 174, 91 ); }
};

///////////////////////////////////////////////////////////////////////////////
