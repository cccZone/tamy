/// @file   TamyEditor/RenderingPipelineLayout.h
/// @brief  layout for a rendering pipeline
#pragma once

#include "TGraphLayout.h"
#include "core-Renderer/RenderingPipeline.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineNode;
class GraphBlock;

///////////////////////////////////////////////////////////////////////////////

/**
 * Layout for a rendering pipeline
 */
class RenderingPipelineLayout : public TGraphLayout< RenderingPipeline, RenderingPipelineNode >
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
   void initBlocksFactory( BlocksFactory& factory );
   void initSocketsFactory( SocketsFactory& factory );
};

///////////////////////////////////////////////////////////////////////////////
