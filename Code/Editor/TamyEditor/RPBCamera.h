/// @file   TamyEditor/RPBCamera.h
/// @brief  rendering pipeline block representing a camera node
#pragma once

#include "core-Renderer/RPCameraNode.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a camera node.
 */
class RPBCamera : public TGraphBlock< RPCameraNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBCamera() : TGraphBlock< RPCameraNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBCamera( RPCameraNode& node );
};

///////////////////////////////////////////////////////////////////////////////
