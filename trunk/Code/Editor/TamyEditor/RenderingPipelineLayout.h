/// @file   TamyEditor/RenderingPipelineLayout.h
/// @brief  layout for a rendering pipeline
#pragma once

#include "GraphLayout.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineNode;
class RenderingPipelineBlock;
class RenderingPipeline;

///////////////////////////////////////////////////////////////////////////////

/**
 * Layout for a rendering pipeline
 */
class RenderingPipelineLayout : public Resource, public GraphLayout, public GenericFactory< RenderingPipelineNode, RenderingPipelineBlock >
{
   DECLARE_RESOURCE( RenderingPipelineLayout )

private:
   RenderingPipeline*                  m_model;

public:
   /**
    * Constructor.
    *
    * @param path
    */
   RenderingPipelineLayout( const std::string& path = "" );

   // -------------------------------------------------------------------------
   // GraphLayout implementation
   // -------------------------------------------------------------------------
   GraphBlock* createNode( const Class& type );
   void removeNode( Object& node );
   void getNodesClasses( std::vector< Class >& classes );

protected:
   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceSave( ResourcesManager& mgr );
   void onResourceLoaded( ResourcesManager& mgr );
};

///////////////////////////////////////////////////////////////////////////////
