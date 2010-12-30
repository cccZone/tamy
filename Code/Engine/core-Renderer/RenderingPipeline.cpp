#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include <algorithm>
#include "core/Algorithms.h"


// TODO: jak juz bedzie wsjo dzialalo, zrobic ten resource const'owym - zrobic cos 
// jak runtime data buffer, gdzie node'y beda przechowywaly sobie dane na ktorych
// operuja, i niech tym buforem bedzie dla nich RenderingPipelineMechanism

///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipeline, Resource, trp, AM_BINARY );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_nodes );
   PROPERTY( std::vector< RenderTargetDescriptor* >, m_renderTargets );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

RenderingPipeline::RenderingPipeline( const std::string& fileName )
   : Resource( fileName )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipeline::~RenderingPipeline()
{
   m_nodes.clear();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::addNode( RenderingPipelineNode* node )
{
   notify( RPO_PRE_CHANGE );

   addObject( node );
   m_nodes.push_back( node );
   
   notify( RPO_POST_CHANGE );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::removeNode( RenderingPipelineNode& node )
{
   std::vector< RenderingPipelineNode* >::iterator it = std::find( m_nodes.begin(), m_nodes.end(), &node );
   if ( it != m_nodes.end() )
   {
      notify( RPO_PRE_CHANGE );

      m_nodes.erase( it );
      removeObject( node.getObjectId() );

      notify( RPO_POST_CHANGE );
   }
   else
   {
      ASSERT_MSG( false, "Trying to remove a non-existent rendering pipeline node" );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::checkGraph()
{
   // TODO : !!!!!!! zrobic sprawdzanie poprawnosci grafu
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
         notify( RPO_PRE_CHANGE );

         delete m_renderTargets[i];
         m_renderTargets.erase( m_renderTargets.begin() + i );

         notify( RPO_POST_CHANGE );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget& RenderingPipeline::getRenderTarget( const std::string& id ) const
{
   RenderTargetDescriptor* desc = findRenderTarget( id );
   if ( desc == NULL )
   {
      char errorMsg[512];
      sprintf_s( errorMsg, "Render target '%s' not found", id.c_str() );
      ASSERT_MSG( desc != NULL, errorMsg );
      throw std::runtime_error( errorMsg );
   }
   
   return desc->getTarget();
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
