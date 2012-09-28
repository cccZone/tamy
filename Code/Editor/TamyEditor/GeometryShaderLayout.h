/// @file   TamyEditor/GeometryShaderLayout.h
/// @brief  geometry shader pipeline layout
#pragma once

#include "TGraphLayout.h"
#include "core-Renderer/GeometryShader.h"


///////////////////////////////////////////////////////////////////////////////

class GeometryShaderNode;
class GraphBlock;

///////////////////////////////////////////////////////////////////////////////

/**
 * Layout for a rendering pipeline
 */
class GeometryShaderLayout : public TGraphLayout< GeometryShader, GeometryShaderNode >
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
   void initBlocksFactory( BlocksFactory& factory );
   void initSocketsFactory( SocketsFactory& factory );
};

///////////////////////////////////////////////////////////////////////////////
