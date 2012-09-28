/// @file   TamyEditor/MaterialLayout.h
/// @brief  material pipeline layout
#pragma once

#include "TGraphLayout.h"
#include "core-Renderer/Material.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialNode;
class GraphBlock;

///////////////////////////////////////////////////////////////////////////////

/**
 * Layout for a rendering pipeline
 */
class MaterialLayout : public TGraphLayout< Material, MaterialNode >
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
   void initBlocksFactory( BlocksFactory& factory );
   void initSocketsFactory( SocketsFactory& factory );
};

///////////////////////////////////////////////////////////////////////////////
