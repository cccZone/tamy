/// @file   TamyEditor/GeometryShaderLayout.h
/// @brief  geometry shader pipeline layout
#pragma once

#include "GraphLayout.h"
#include "PipelineLayout.h"
#include "core-Renderer/GeometryShader.h"


///////////////////////////////////////////////////////////////////////////////

class GeometryShaderNode;
class PipelineBlock;

///////////////////////////////////////////////////////////////////////////////

/**
 * Layout for a rendering pipeline
 */
class GeometryShaderLayout : public PipelineLayout< GeometryShader, GeometryShaderNode >
{
   DECLARE_RESOURCE()

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   GeometryShaderLayout( const FilePath& resourceName = FilePath() );

protected:
   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void initBlocksFactory();
   void initSocketsFactory( PipelineBlock& block );
};

///////////////////////////////////////////////////////////////////////////////
