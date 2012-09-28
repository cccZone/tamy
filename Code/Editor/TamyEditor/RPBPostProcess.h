/// @file   TamyEditor/RPBPostProcess.h
/// @brief  rendering pipeline block representing a single post process stage
#pragma once

#include "core-Renderer/RPPostProcessNode.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPBPostProcess : public TGraphBlock< RPPostProcessNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBPostProcess() : TGraphBlock< RPPostProcessNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBPostProcess( RPPostProcessNode& node );
};

///////////////////////////////////////////////////////////////////////////////
