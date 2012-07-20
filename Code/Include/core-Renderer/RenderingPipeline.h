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
class RuntimeDataBuffer;

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

   // runtime data
   std::string                                  m_lockedRT;

public:
   /**
    * Constructor.
    *
    * @param resourceName   name of the resource file
    */
   RenderingPipeline( const FilePath& resourceName = FilePath() );
   ~RenderingPipeline();

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

protected:
   // -------------------------------------------------------------------------
   // GraphBuilder implementation
   // -------------------------------------------------------------------------
   void onNodeAdded( RenderingPipelineNode* node );
   void onNodeRemoved( RenderingPipelineNode& node );

};

///////////////////////////////////////////////////////////////////////////////
