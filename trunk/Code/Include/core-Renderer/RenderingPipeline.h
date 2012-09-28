/// @file   core-Renderer/RenderingPipeline.h
/// @brief  rendering pipeline resource
#pragma once

#include "core/Resource.h"
#include "core/GraphBuilder.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineNode;
class RenderingPipelineMechanism;
class RenderTargetDescriptor;
class RenderTarget;
class DepthBufferDescriptor;
class DepthBuffer;
class RuntimeDataBuffer;
class RenderingPipelineTransaction;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline represents a structure that performs all rendering related
 * image processing and presents the final image in the back buffer.
 */
class RenderingPipeline : public Resource, public GraphBuilder< RenderingPipeline, RenderingPipelineNode >
{
   DECLARE_RESOURCE();

private:
   std::vector< RenderTargetDescriptor* >       m_renderTargets;
   std::vector< DepthBufferDescriptor* >        m_depthBuffers;

   // runtime data
   std::string                                  m_lockedRT;
   std::string                                  m_lockedDB;

public:
   /**
    * Constructor.
    *
    * @param resourceName   name of the resource file
    */
   RenderingPipeline( const FilePath& resourceName = FilePath() );
   ~RenderingPipeline();

   // -------------------------------------------------------------------------
   // GraphBuilder implementation
   // -------------------------------------------------------------------------
   /**
    * Creates a transaction that will change this graph builder instance.
    *
    * @param pipeline
    */
   static GraphBuilderTransaction< RenderingPipeline, RenderingPipelineNode >* createTransaction( RenderingPipeline& pipeline );

   // -------------------------------------------------------------------------
   // Render targets management
   // -------------------------------------------------------------------------
   /**
    * Returns a render target registered under the specified ID.
    *
    * @param id            render target id
    * @param runtimeData   runtime data buffer the target is stored in
    */
   RenderTarget* getRenderTarget( const std::string& id, RuntimeDataBuffer& runtimeData ) const;

   /**
    * Looks for a render target descriptor with the specified id.
    *
    * @param id         render target id
    */
   RenderTargetDescriptor* findRenderTarget( const std::string& id ) const;

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

   // -------------------------------------------------------------------------
   // Depth buffers management
   // -------------------------------------------------------------------------

   /**
    * Returns a depth buffer registered under the specified ID.
    *
    * @param id            depth buffer id
    * @param runtimeData   runtime data buffer the depth buffer is stored in
    */
   DepthBuffer* getDepthBuffer( const std::string& id, RuntimeDataBuffer& runtimeData ) const;

   /**
    * Looks for a depth buffer descriptor with the specified id.
    *
    * @param id         depth buffer id
    */
   DepthBufferDescriptor* findDepthBuffer( const std::string& id ) const;

   /**
    * Returns a list of all defined depth buffers.
    */
   inline const std::vector< DepthBufferDescriptor* >& getDepthBuffers() const { return m_depthBuffers; }
  
   /**
    * Locks the specified depth buffer descriptor, allowing to change it.
    *
    * @param id         depth buffer id
    */
   DepthBufferDescriptor& lockDepthBuffer( const std::string& id );

   /**
    * Unlocks the specified depth buffer.
    */
   void unlockDepthBuffer( const std::string& id );

protected:
   // -------------------------------------------------------------------------
   // GraphBuilder implementation
   // -------------------------------------------------------------------------
   void onNodeAdded( RenderingPipelineNode* node );
   void onNodeRemoved( RenderingPipelineNode& node );

private:
   // -------------------------------------------------------------------------
   // Rendering pipeline transaction API
   // -------------------------------------------------------------------------
   friend class RenderingPipelineTransaction;

   /**
    * Adds a new depth buffer definition.
    *
    * @param descriptor    depth buffer descriptor
    * @return              'true' if the depth buffer was added successfully, 'false' otherwise
    */
   void addDepthBuffer( DepthBufferDescriptor* descriptor );

   /**
    * Removes the specified depth buffer descriptor.
    *
    * @param id         depth buffer id
    */
   void removeDepthBuffer( const std::string& id );

   /**
    * Adds a new render target definition.
    *
    * @param descriptor    render target descriptor
    * @return              'true' if the target was added successfully, 'false' otherwise
    */
   void addRenderTarget( RenderTargetDescriptor* descriptor );

   /**
    * Removes the specified render target descriptor.
    *
    * @param id         render target id
    */
   void removeRenderTarget( const std::string& id );

};

///////////////////////////////////////////////////////////////////////////////
