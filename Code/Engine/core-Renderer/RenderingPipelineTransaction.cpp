#include "core-Renderer\RenderingPipelineTransaction.h"
#include "core-Renderer\RenderTargetDescriptor.h"
#include "core-Renderer\DepthBufferDescriptor.h"
#include "core-Renderer\RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

bool RenderingPipelineTransaction::addRenderTarget( RenderTargetDescriptor* descriptor )
{
   if ( !descriptor )
   {
      return false;
   }

   // check for duplicates
   uint count = m_renderTargetsToAdd.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_renderTargetsToAdd[i] == descriptor )
      {
         // it's already here, so it won't be added again
         return false;
      }
   }

   m_renderTargetsToAdd.push_back( descriptor );
   return true;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineTransaction::removeRenderTarget( const std::string& id )
{
   if ( id.empty() )
   {
      return;
   }

   // check for duplicates
   uint count = m_renderTargetsToRemove.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_renderTargetsToRemove[i] == id )
      {
         // it's already here
         return;
      }
   }

   m_renderTargetsToRemove.push_back( id );
}

///////////////////////////////////////////////////////////////////////////////

bool RenderingPipelineTransaction::addDepthBuffer( DepthBufferDescriptor* descriptor )
{
   if ( !descriptor )
   {
      return false;
   }

   // check for duplicates
   uint count = m_depthBuffersToAdd.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_depthBuffersToAdd[i] == descriptor )
      {
         // it's already here, so it won't be added again
         return false;
      }
   }

   m_depthBuffersToAdd.push_back( descriptor );
   return true;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineTransaction::removeDepthBuffer( const std::string& id )
{
   if ( id.empty() )
   {
      return;
   }

   // check for duplicates
   uint count = m_depthBuffersToRemove.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_depthBuffersToRemove[i] == id )
      {
         // it's already here
         return;
      }
   }

   m_depthBuffersToRemove.push_back( id );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineTransaction::onCommit( RenderingPipeline& graph )
{
   // run prerequisites check on new nodes
   uint count = m_nodesToAdd.size();
   for ( uint i = 0; i < count; ++i )
   {
      m_nodesToAdd[i]->onCreatePrerequisites( *this );
   }

   // remove obsolete render targets
   count = m_renderTargetsToRemove.size();
   for ( uint i = 0; i < count; ++i )
   {
      graph.removeRenderTarget( m_renderTargetsToRemove[i] );
   } 

   // remove obsolete depth buffers
   count = m_depthBuffersToRemove.size();
   for ( uint i = 0; i < count; ++i )
   {
      graph.removeDepthBuffer( m_depthBuffersToRemove[i] );
   } 

   // add new render targets
   count = m_renderTargetsToAdd.size();
   for ( uint i = 0; i < count; ++i )
   {
      graph.addRenderTarget( m_renderTargetsToAdd[i] );
   } 

   // add new depth buffers
   count = m_depthBuffersToAdd.size();
   for ( uint i = 0; i < count; ++i )
   {
      graph.addDepthBuffer( m_depthBuffersToAdd[i] );
   } 
}

///////////////////////////////////////////////////////////////////////////////
