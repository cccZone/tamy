/// @file   TamyEditor/RPBPreview.h
/// @brief  rendering pipeline block representing a preview node
#pragma once

#include "core-Renderer/RPPreviewNode.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPBPreview : public TGraphBlock< RPPreviewNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBPreview() : TGraphBlock< RPPreviewNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBPreview( RPPreviewNode& node );
};

///////////////////////////////////////////////////////////////////////////////
