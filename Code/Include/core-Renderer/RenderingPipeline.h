/// @file   core-Renderer/RenderingPipeline.h
/// @brief  rendering pipeline resource

#pragma once

#include "core/Resource.h"
#include <vector>
#include "core/Subject.h"
#include "core/Graph.h"

///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineNode;
class RenderingPipelineMechanism;
class RenderTargetDescriptor;
class RenderTarget;
class RuntimeDataBuffer;
typedef Graph< RenderingPipelineNode* > RPGraph;

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

   // runtime data
   std::string                                  m_lockedRT;

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
    * Constructs a graph based on the pipeline.
    *
    * @param outGraph      resulting graph
    */
   void buildGraph( RPGraph& outGraph ) const;

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
    * @param id            render target id
    * @param runtimeData   runtime data buffer the target is stored in
    */
   RenderTarget& getRenderTarget( const std::string& id, RuntimeDataBuffer& runtimeData ) const;

   /**
    * Returns a list of all defined render targets.
    */
   inline const std::vector< RenderTargetDescriptor* >& getRenderTargets() const { return m_renderTargets; }
  
   /**
    * Locks the specified render target descriptor, allowing to change it.
    *
    * @param id         render target id
    */
   RenderTargetDescriptor& lockRenderTarget( const std::string& id );

   /**
    * Unlocks the specified render target.
    */
   void unlockRenderTarget( const std::string& id );

private:
   /**
    * Looks for a render target descriptor with the specified id.
    *
    * @param id         render target id
    */
   RenderTargetDescriptor* findRenderTarget( const std::string& id ) const;
};

///////////////////////////////////////////////////////////////////////////////
