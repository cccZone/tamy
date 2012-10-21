/// @file   core-Renderer/RenderingPipelineTransaction.h
/// @brief  RenderingPipeline graph transaction
#pragma once

#include "core/GraphBuilderTransaction.h"
#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTargetDescriptor;
class DepthBufferDescriptor;

///////////////////////////////////////////////////////////////////////////////

/**
 * RenderingPipeline graph transaction.
 */
class RenderingPipelineTransaction : public GraphBuilderTransaction< RenderingPipeline, RenderingPipelineNode >
{
   DECLARE_ALLOCATOR( RenderingPipelineTransaction, AM_DEFAULT );

private:
   std::vector< RenderTargetDescriptor* >       m_renderTargetsToAdd;
   std::vector< std::string >                   m_renderTargetsToRemove;

   std::vector< DepthBufferDescriptor* >        m_depthBuffersToAdd;
   std::vector< std::string >                   m_depthBuffersToRemove;

public:
   /**
    * Constructor.
    *
    * @param pipeline
    */
   RenderingPipelineTransaction( RenderingPipeline& pipeline );

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
    * Adds a new depth buffer definition.
    *
    * @param descriptor    depth buffer descriptor
    * @return              'true' if the depth buffer was added successfully, 'false' otherwise
    */
   bool addDepthBuffer( DepthBufferDescriptor* descriptor );

   /**
    * Removes the specified depth buffer descriptor.
    *
    * @param id         depth buffer id
    */
   void removeDepthBuffer( const std::string& id );

protected:
   // -------------------------------------------------------------------------
   // GraphBuilderTransaction implementation
   // -------------------------------------------------------------------------
   void onCommit( RenderingPipeline& graph );
};

///////////////////////////////////////////////////////////////////////////////
