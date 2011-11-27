/// @file   core-Renderer/GeometryShader.h
/// @brief  geometry shader pipeline resource
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilder.h"


///////////////////////////////////////////////////////////////////////////////

class GeometryShaderNode;
class Renderer;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

class GeometryShader : public Resource, public GraphBuilder< GeometryShader, GeometryShaderNode >
{
   DECLARE_RESOURCE( GeometryShader )

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   GeometryShader( const FilePath& resourceName = FilePath() );
   ~GeometryShader();

protected:
   // -------------------------------------------------------------------------
   // GraphBuilder implementation
   // -------------------------------------------------------------------------
   void onNodeAdded( GeometryShaderNode* node );
   void onNodeRemoved( GeometryShaderNode& node );
};

///////////////////////////////////////////////////////////////////////////////
