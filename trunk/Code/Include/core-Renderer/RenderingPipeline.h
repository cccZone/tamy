/// @file   core-Renderer/RenderingPipeline.h
/// @brief  rendering pipeline resource

#pragma once

#include "core/Resource.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineNode;
class RenderingPipelineMechanism;
class RenderTargetDescriptor;
class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline represents a structure that performs all rendering related
 * image processing and presents the final image in the back buffer.
 */
class RenderingPipeline : public Resource
{
   DECLARE_RESOURCE( RenderingPipeline );

private:
   std::vector< RenderingPipelineNode* >        m_nodes;
   std::vector< RenderTargetDescriptor* >       m_renderTargets;

   // runtime queue of the nodes
   std::vector< RenderingPipelineNode* >        m_nodesQueue;

public:
   /**
    * Constructor.
    *
    * @param fileName   name of the resource file
    */
   RenderingPipeline( const std::string& fileName = "" );
   ~RenderingPipeline();

   // -------------------------------------------------------------------------
   // Nodes management
   // -------------------------------------------------------------------------
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

   /**
    * Performs a structural check of the graph and flags all inconsistencies.
    */
   void checkGraph();

   // -------------------------------------------------------------------------
   // Render targets management
   // -------------------------------------------------------------------------
   /**
    * Adds a new render target definition.
    *
    * @param descriptor    render target descriptor
    * @return              'true' if the target was added successfully, 'false' otherwise
    */
   bool addRenderTarget( RenderTargetDescriptor* descriptor );

   /**
    * Removes the specified render target descriptor.
    *
    * @param id         render target id
    */
   void removeRenderTarget( const std::string& id );

   /**
    * Returns a render target registered under the specified ID.
    *
    * @param id         render target id
    */
   RenderTarget& getRenderTarget( const std::string& id ) const;

   /**
    * Returns a list of all defined render targets.
    */
   inline const std::vector< RenderTargetDescriptor* >& getRenderTargets() const { return m_renderTargets; }

   // -------------------------------------------------------------------------
   // Pipeline runtime management
   // -------------------------------------------------------------------------
   /**
    * Initializes the pipeline for the use with the specified rendering pass.
    *
    * @param host       host rendering pass
    */
   void initialize( RenderingPipelineMechanism& host );

   /**
    * Deinitializes the pipeline.
    *
    * @param host       host rendering pass
    */
   void deinitialize( RenderingPipelineMechanism& host );

   /**
    * Renders using the pipeline.
    *
    * @param host       rendering pass the pipeline will be used to render
    */
   void render( RenderingPipelineMechanism& host );

private:
   /**
    * Caches the nodes for quicker processing.
    */
   void cacheNodes();

    /**
    * Looks for a render target descriptor with the specified id.
    *
    * @param id         render target id
    */
   RenderTargetDescriptor* findRenderTarget( const std::string& id ) const;
};

///////////////////////////////////////////////////////////////////////////////
