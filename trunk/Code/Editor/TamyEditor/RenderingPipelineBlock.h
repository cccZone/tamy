/// @file   TamyEditor/RenderingPipelineBlock.h
/// @brief  rendering pipeline block representation base
#pragma once

#include "GraphBlock.h"
#include "core-Renderer.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipeline;

///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineBlock : public GraphBlock
{
   DECLARE_CLASS( RenderingPipelineBlock )

public:
   /**
    * Default constructor.
    */
   RenderingPipelineBlock() {}

   /**
    * Constructor.
    *
    * @param node       represented node
    */
   RenderingPipelineBlock( RenderingPipelineNode& node );

   /**
    * Called in order to initialize the block
    *
    * @param parentResource      resource that manages the object the block represents
    */
   virtual void initialize( RenderingPipeline& parentResource ) {}
};

///////////////////////////////////////////////////////////////////////////////
