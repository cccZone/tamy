/// @file   TamyEditor/RenderingPipelineLayout.h
/// @brief  layout for a rendering pipeline
#pragma once

#include "GraphLayout.h"
#include "PipelineLayout.h"
#include "core-Renderer/RenderingPipeline.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineNode;
class PipelineBlock;

///////////////////////////////////////////////////////////////////////////////

/**
 * Layout for a rendering pipeline
 */
class RenderingPipelineLayout : public PipelineLayout< RenderingPipeline, RenderingPipelineNode >
{
   DECLARE_RESOURCE()

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   RenderingPipelineLayout( const FilePath& resourceName = FilePath() );

protected:
   // -------------------------------------------------------------------------
   // PipelineLayout implementation
   // -------------------------------------------------------------------------
   void initBlocksFactory();
   void initSocketsFactory( PipelineBlock& block );
};

///////////////////////////////////////////////////////////////////////////////
