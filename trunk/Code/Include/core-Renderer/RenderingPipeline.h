/// @file   core-Renderer/RenderingPipeline.h
/// @brief  rendering pipeline resource

#pragma once

#include "core/Resource.h"
#include <vector>
#include "core/Subject.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineNode;
class RenderingPipelineMechanism;
class RenderTargetDescriptor;
class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

enum RenderingPipelineOperation
{
   RPO_PRE_CHANGE,
   RPO_POST_CHANGE
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline represents a structure that performs all rendering related
 * image processing and presents the final image in the back buffer.
 */
class RenderingPipeline : public Resource, public Subject< RenderingPipeline, RenderingPipelineOperation >
{
   DECLARE_RESOURCE( RenderingPipeline );

private:
   std::vector< RenderingPipelineNode* >        m_nodes;
   std::vector< RenderTargetDescriptor* >       m_renderTargets;

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
    * Returns a list of all defined nodes.
    */
   inline const std::vector< RenderingPipelineNode* >& getNodes() const { return m_nodes; }

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
  

    /**
    * Looks for a render target descriptor with the specified id.
    *
    * @param id         render target id
    */
   RenderTargetDescriptor* findRenderTarget( const std::string& id ) const;
};

///////////////////////////////////////////////////////////////////////////////
