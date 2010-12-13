/// @file   core-Renderer/RenderingPipeline.h
/// @brief  rendering pipeline resource

#pragma once

#include "core/Resource.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node.
 */
class RenderingPipelineNode : public ResourceObject
{
   DECLARE_CLASS( RenderingPipelineNode )
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline represents a structure that performs all rendering related
 * image processing and presents the final image in the back buffer.
 */
class RenderingPipeline : public Resource
{
   DECLARE_RESOURCE( RenderingPipeline );

private:
   std::vector< RenderingPipelineNode* >     m_nodes;

public:
   /**
    * Constructor.
    *
    * @param fileName   name of the resource file
    */
   RenderingPipeline( const std::string& fileName = "" );
   ~RenderingPipeline();

   /**
    * Adds a new node to the pipeline.
    *
    * @param node       node to add
    */
   void addNode( RenderingPipelineNode* node );

   /**
    * Removes a node from the pipeline
    *
    * @param node       node to remove
    */
   void removeNode( RenderingPipelineNode& node );
};

///////////////////////////////////////////////////////////////////////////////
