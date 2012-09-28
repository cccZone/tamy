/// @file   core-Renderer/Material.h
/// @brief  material resource
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilder.h"
#include "core/GraphBuilderTransaction.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialEntity;
class MaterialNode;
class Renderer;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

class Material : public Resource, public GraphBuilder< Material, MaterialNode >
{
   DECLARE_RESOURCE()

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   Material( const FilePath& resourceName = FilePath() );
   ~Material();

   // -------------------------------------------------------------------------
   // GraphBuilder implementation
   // -------------------------------------------------------------------------
   /**
    * Creates a transaction that will change this graph builder instance.
    */
   static GraphBuilderTransaction< Material, MaterialNode >* createTransaction( Material& material ) { return new GraphBuilderTransaction< Material, MaterialNode >( material ); }

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );

protected:
   // -------------------------------------------------------------------------
   // GraphBuilder implementation
   // -------------------------------------------------------------------------
   void onNodeAdded( MaterialNode* node );
   void onNodeRemoved( MaterialNode& node );
};

///////////////////////////////////////////////////////////////////////////////
