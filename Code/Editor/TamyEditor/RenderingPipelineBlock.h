/// @file   TamyEditor/RenderingPipelineBlock.h
/// @brief  rendering pipeline block representation base
#pragma once

#include "GraphBlock.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipeline;

///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineBlock : public GraphBlock
{
   DECLARE_CLASS( RenderingPipelineBlock )

public:
   /**
    * Called in order to initialize the block
    *
    * @param parentResource      resource that manages the object the block represents
    */
   virtual void initialize( RenderingPipeline& parentResource ) {}
};

///////////////////////////////////////////////////////////////////////////////
