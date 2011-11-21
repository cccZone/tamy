#include "RenderingPipelineLayout.h"
#include "core.h"
#include "core-Renderer.h"

// blocks
#include "RPBPostProcess.h"
#include "RPBSceneRender.h"
#include "RPBDebugRender.h"
#include "RPBStart.h"
#include "RPBAdapter.h"
#include "RPBFloats2Vec4.h"
#include "RPBCamera.h"
#include "RPBTexture.h"
#include "RPBFloat.h"
#include "RPBVec4.h"

///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipelineLayout, Resource, rpl, AM_BINARY );
   PROPERTY( RenderingPipeline*, m_model );
   PROPERTY( std::vector< GraphBlock* >, m_blocks );
   PROPERTY( std::vector< GraphBlockConnection* >, m_connections );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineLayout::RenderingPipelineLayout( const FilePath& resourceName )
   : Resource( resourceName )
   , m_model( NULL )
{
   // create block-to-node associations
   associate< RPSceneRenderNode, RPBSceneRender >();
   associate< RPDebugRendererNode, RPBDebugRender >();
   associate< RPStartNode, RPBStart >();
   associate< RPAdapterNode, RPBAdapter >();
   associate< RPPostProcessNode, RPBPostProcess >();
   associate< RPFloats2Vec4Node, RPBFloats2Vec4 >();
   associate< RPCameraNode, RPBCamera >();
   associate< RPTextureNode, RPBTexture >();
   associate< RPFloatNode, RPBFloat >();
   associate< RPVec4Node, RPBVec4 >();

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
      std::string resourcePath = getFilePath();
      m_model = mgr.findResource< RenderingPipeline >( resourcePath );
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
