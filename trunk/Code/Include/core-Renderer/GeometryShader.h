/// @file   core-Renderer/GeometryShader.h
/// @brief  geometry shader pipeline resource
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilder.h"
#include "core/GraphBuilderTransaction.h"


///////////////////////////////////////////////////////////////////////////////

class GeometryShaderNode;
class Renderer;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

class GeometryShader : public Resource, public GraphBuilder< GeometryShader, GeometryShaderNode >
{
   DECLARE_RESOURCE()

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   GeometryShader( const FilePath& resourceName = FilePath() );
   ~GeometryShader();

   // -------------------------------------------------------------------------
   // GraphBuilder implementation
   // -------------------------------------------------------------------------
   /**
    * Creates a transaction that will change this graph builder instance.
    */
   static GraphBuilderTransaction< GeometryShader, GeometryShaderNode >* createTransaction() { return new GraphBuilderTransaction< GeometryShader, GeometryShaderNode >(); }

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );

protected:
   // -------------------------------------------------------------------------
   // GraphBuilder implementation
   // -------------------------------------------------------------------------
   void onNodeAdded( GeometryShaderNode* node );
   void onNodeRemoved( GeometryShaderNode& node );
};

///////////////////////////////////////////////////////////////////////////////
