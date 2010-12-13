#include "core-Renderer/RenderingPipeline.h"
#include <algorithm>
#include "core/Algorithms.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RenderingPipelineNode, ResourceObject );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( RenderingPipeline, Resource, trp, AM_BINARY );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_nodes );
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
   addObject( node );
   m_nodes.push_back( node );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipeline::removeNode( RenderingPipelineNode& node )
{
   std::vector< RenderingPipelineNode* >::iterator it = std::find( m_nodes.begin(), m_nodes.end(), &node );
   if ( it != m_nodes.end() )
   {
      m_nodes.erase( it );
      removeObject( node.getObjectId() );
   }
   else
   {
      ASSERT_MSG( false, "Trying to remove a non-existent rendering pipeline node" );
   }
}

///////////////////////////////////////////////////////////////////////////////
