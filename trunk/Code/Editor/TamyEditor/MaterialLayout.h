/// @file   TamyEditor/MaterialLayout.h
/// @brief  material pipeline layout
#pragma once

#include "GraphLayout.h"
#include "PipelineLayout.h"
#include "core-Renderer/Material.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialNode;
class PipelineBlock;

///////////////////////////////////////////////////////////////////////////////

/**
 * Layout for a rendering pipeline
 */
class MaterialLayout : public PipelineLayout< Material, MaterialNode >
{
   DECLARE_RESOURCE()

public:
   /**
    * Constructor.
    *
    * @param resourceName
    */
   MaterialLayout( const FilePath& resourceName = FilePath() );

protected:
   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void initBlocksFactory();
   void initSocketsFactory( PipelineBlock& block );
};

///////////////////////////////////////////////////////////////////////////////
