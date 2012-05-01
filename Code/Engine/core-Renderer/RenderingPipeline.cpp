#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RPStartNode.h"
#include <algorithm>
#include "core/Algorithms.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipeline, trp, AM_BINARY );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_nodes );
   PROPERTY( std::vector< RenderTargetDescriptor* >, m_renderTargets );
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

void RenderingPipeline::onNodeAdded( RenderingPipelineNode* node )
{
   addObject( node );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::onNodeRemoved( RenderingPipelineNode& node )
{
   removeObject( node.getObjectId() );
}

///////////////////////////////////////////////////////////////////////////////

bool RenderingPipeline::addRenderTarget( RenderTargetDescriptor* descriptor )
{
   if ( descriptor == NULL )
   {
      ASSERT_MSG( descriptor != NULL, "NULL pointer specified instead a valid render target descriptor" );
      return false;
   }

   // check if the specified render target doesn't share an id with already registered one
   RenderTargetDescriptor* existingDescriptor = findRenderTarget( descriptor->getTargetID() );
   if ( existingDescriptor != NULL )
   {
      ASSERT_MSG( descriptor == NULL, "The specified descriptor has an ID used by another render target." );
      delete descriptor;
      return false;
   }

   m_renderTargets.push_back( descriptor );
   return true;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::removeRenderTarget( const std::string& id )
{
   unsigned int count = m_renderTargets.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_renderTargets[i]->getTargetID() == id )
      {
         notify( GBO_PRE_CHANGE );

         delete m_renderTargets[i];
         m_renderTargets.erase( m_renderTargets.begin() + i );

         notify( GBO_POST_CHANGE );
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
      throw std::runtime_error( "Another render target is already locked" );
   }

   RenderTargetDescriptor* desc = findRenderTarget( id );
   if ( desc == NULL )
   {
      char errorMsg[512];
      sprintf_s( errorMsg, "Render target '%s' not found", id.c_str() );
      ASSERT_MSG( desc != NULL, errorMsg );
      throw std::runtime_error( errorMsg );
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
      throw std::runtime_error( "Trying to unlock a render target that hasn't been locked" );
   }
   m_lockedRT = "";
   notify( GBO_POST_CHANGE );
}

///////////////////////////////////////////////////////////////////////////////
