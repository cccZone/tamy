#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/DepthBufferDescriptor.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineTransaction.h"
#include "core-Renderer/RPStartNode.h"
#include <algorithm>
#include "core/Algorithms.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipeline, trp, AM_BINARY );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_nodes );
   PROPERTY( std::vector< RenderTargetDescriptor* >, m_renderTargets );
   PROPERTY( std::vector< DepthBufferDescriptor* >, m_depthBuffers );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

RenderingPipeline::RenderingPipeline( const FilePath& resourceName )
   : Resource( resourceName )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipeline::~RenderingPipeline()
{
}

///////////////////////////////////////////////////////////////////////////////

GraphBuilderTransaction< RenderingPipeline, RenderingPipelineNode >* RenderingPipeline::createTransaction()
{
   return new RenderingPipelineTransaction();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::onNodeAdded( RenderingPipelineNode* node )
{
   if ( node )
   {
      addObject( node );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::onNodeRemoved( RenderingPipelineNode& node )
{
   removeObject( node.getObjectId() );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::addRenderTarget( RenderTargetDescriptor* descriptor )
{
   ASSERT_MSG( descriptor != NULL, "NULL pointer specified instead a valid render target descriptor" );

   // check if the specified render target doesn't share an id with already registered one
   RenderTargetDescriptor* existingDescriptor = findRenderTarget( descriptor->getTargetID() );
   if ( existingDescriptor != NULL )
   {
      delete descriptor;
      return;
   }

   m_renderTargets.push_back( descriptor );  
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::removeRenderTarget( const std::string& id )
{
   unsigned int count = m_renderTargets.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_renderTargets[i]->getTargetID() == id )
      {
         delete m_renderTargets[i];
         m_renderTargets.erase( m_renderTargets.begin() + i );

         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget* RenderingPipeline::getRenderTarget( const std::string& id, RuntimeDataBuffer& runtimeData ) const
{
   RenderTargetDescriptor* desc = findRenderTarget( id );
   if ( desc == NULL )
   {
      // we want to render directly to the back buffer
      return NULL;
   }
   else
   {
      return desc->getTarget( runtimeData );
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptor* RenderingPipeline::findRenderTarget( const std::string& id ) const
{
   unsigned int count = m_renderTargets.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_renderTargets[i]->getTargetID() == id )
      {
         return m_renderTargets[i];
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

RenderTargetDescriptor& RenderingPipeline::lockRenderTarget( const std::string& id )
{
   if ( !m_lockedRT.empty() )
   {
      ASSERT_MSG( false, "Another render target is already locked" );
   }

   RenderTargetDescriptor* desc = findRenderTarget( id );
   if ( desc == NULL )
   {
      char errorMsg[512];
      sprintf_s( errorMsg, "Render target '%s' not found", id.c_str() );
      ASSERT_MSG( desc != NULL, errorMsg );
   }

   m_lockedRT = id;
   notify( GBO_PRE_CHANGE );
   return *desc;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::unlockRenderTarget( const std::string& id )
{
   if ( m_lockedRT.empty() || m_lockedRT != id )
   {
      ASSERT_MSG( false, "Trying to unlock a render target that hasn't been locked" );
   }
   m_lockedRT = "";
   notify( GBO_POST_CHANGE );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::addDepthBuffer( DepthBufferDescriptor* descriptor )
{
   ASSERT_MSG( descriptor != NULL, "NULL pointer specified instead a valid depth buffer descriptor" );

   // check if the specified depth buffer doesn't share an id with already registered one
   DepthBufferDescriptor* existingDescriptor = findDepthBuffer( descriptor->getID() );
   if ( existingDescriptor != NULL )
   {
      delete descriptor;
      return;
   }

   m_depthBuffers.push_back( descriptor );  
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::removeDepthBuffer( const std::string& id )
{
   unsigned int count = m_depthBuffers.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_depthBuffers[i]->getID() == id )
      {
         delete m_depthBuffers[i];
         m_depthBuffers.erase( m_depthBuffers.begin() + i );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

DepthBuffer* RenderingPipeline::getDepthBuffer( const std::string& id, RuntimeDataBuffer& runtimeData ) const
{
   DepthBufferDescriptor* desc = findDepthBuffer( id );
   if ( desc == NULL )
   {
      // we want to render directly to the back buffer
      return NULL;
   }
   else
   {
      return desc->getBuffer( runtimeData );
   }
}

///////////////////////////////////////////////////////////////////////////////

DepthBufferDescriptor* RenderingPipeline::findDepthBuffer( const std::string& id ) const
{
   unsigned int count = m_depthBuffers.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_depthBuffers[i]->getID() == id )
      {
         return m_depthBuffers[i];
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

DepthBufferDescriptor& RenderingPipeline::lockDepthBuffer( const std::string& id )
{
   if ( !m_lockedDB.empty() )
   {
      ASSERT_MSG( false, "Another depth buffer is already locked" );
   }

   DepthBufferDescriptor* desc = findDepthBuffer( id );
   if ( desc == NULL )
   {
      char errorMsg[512];
      sprintf_s( errorMsg, "Depth buffer '%s' not found", id.c_str() );
      ASSERT_MSG( desc != NULL, errorMsg );
   }

   m_lockedDB = id;
   notify( GBO_PRE_CHANGE );
   return *desc;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::unlockDepthBuffer( const std::string& id )
{
   if ( m_lockedDB.empty() || m_lockedDB != id )
   {
      ASSERT_MSG( false, "Trying to unlock a depth buffer that hasn't been locked" );
   }
   m_lockedDB = "";
   notify( GBO_POST_CHANGE );
}

///////////////////////////////////////////////////////////////////////////////
