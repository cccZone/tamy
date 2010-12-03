#include "RenderingPipelineLayout.h"
#include "core.h"
#include "core-Renderer/RenderingPipeline.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipelineLayout, Resource, rpl, AM_BINARY );
   PROPERTY( Resource*, m_model );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineLayout::RenderingPipelineLayout( const std::string& path )
   : Resource( path )
   , m_model( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineLayout::onResourceSave( ResourcesManager& mgr )
{
   saveState();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineLayout::onResourceLoaded( ResourcesManager& mgr ) 
{
   if ( m_model == NULL )
   {
      // the model was not set - create an instance
      std::string resourcePath = Filesystem::changeFileExtension( getFilePath(), RenderingPipeline::getExtension() );

      m_model = dynamic_cast< RenderingPipeline* >( mgr.findResource( resourcePath ) );
      if ( m_model == NULL )
      {
      
         m_model = new RenderingPipeline( resourcePath );
         mgr.addResource( m_model );
      }
   }

   // initialize the layout
   restoreState();
}

///////////////////////////////////////////////////////////////////////////////
