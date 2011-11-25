/// @file   core-Renderer/Material.h
/// @brief  material resource
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilder.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialEntity;
class MaterialNode;
class Renderer;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

class Material : public Resource, public GraphBuilder< Material, MaterialNode >
{
   DECLARE_RESOURCE( Material )

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   Material( const FilePath& resourceName = FilePath() );
   ~Material();

protected:
   // -------------------------------------------------------------------------
   // GraphBuilder implementation
   // -------------------------------------------------------------------------
   void onNodeAdded( MaterialNode* node );
   void onNodeRemoved( MaterialNode& node );
};

///////////////////////////////////////////////////////////////////////////////
