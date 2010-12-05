#include "RenderingPipelineLayout.h"
#include "core.h"
#include "core-Renderer/RenderingPipeline.h"

// nodes
#include "core-Renderer/RPPostProcessNode.h"

// blocks
#include "RPBPostProcess.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipelineLayout, Resource, rpl, AM_BINARY );
   PROPERTY( RenderingPipeline*, m_model );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineLayout::RenderingPipelineLayout( const std::string& path )
   : Resource( path )
   , m_model( NULL )
{
   associate< RPPostProcessNode, RPBPostProcess >();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineLayout::onResourceSave( ResourcesManager& mgr )
{
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

   restoreState();
}

///////////////////////////////////////////////////////////////////////////////

GraphBlock* RenderingPipelineLayout::createNode( const Class& type )
{
   RenderingPipelineNode* node = type.instantiate< RenderingPipelineNode >();
   GraphBlock* block = create( *node );
   m_model->addNode( node );

   return block;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineLayout::removeNode( Object& node )
{
   RenderingPipelineNode* rpNode = dynamic_cast< RenderingPipelineNode* >( &node );
   ASSERT_MSG( rpNode != NULL, "Attempting to remove a node that's not a RenderingPipelineNode" );

   m_model->removeNode( *rpNode );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineLayout::getNodesClasses( std::vector< Class >& classes )
{
   ClassesRegistry& classesReg = getClassesRegistry();
   classesReg.getClassesMatchingType< RenderingPipelineNode >( classes );
}

///////////////////////////////////////////////////////////////////////////////
