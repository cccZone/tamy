#include "RenderingPipelineLayout.h"
#include "core.h"
#include "core-Renderer.h"

// blocks
#include "RPBPostProcess.h"
#include "RPBSceneRender.h"
#include "RPBDepthNormals.h"
#include "RPBDebugRender.h"
#include "RPBStart.h"
#include "RPBAdapter.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipelineLayout, Resource, rpl, AM_BINARY );
   PROPERTY( RenderingPipeline*, m_model );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineLayout::RenderingPipelineLayout( const std::string& path )
   : Resource( path )
   , m_model( NULL )
{
   // create block-to-node associations
   associate< RPSceneRenderNode, RPBSceneRender >();
   associate< RPDepthNormalsNode, RPBDepthNormals >();
   associate< RPDebugRendererNode, RPBDebugRender >();
   associate< RPStartNode, RPBStart >();
   associate< RPAdapterNode, RPBAdapter >();
   associateAbstract< RPPostProcessNode, RPBPostProcess >();

   // customize the looks
   setBackgroundBrush( QColor( 209, 226, 165 ) );
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineLayout::~RenderingPipelineLayout()
{
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
   RenderingPipelineBlock* block = create( *node );
   m_model->addNode( node );
   block->initialize( *m_model );

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
