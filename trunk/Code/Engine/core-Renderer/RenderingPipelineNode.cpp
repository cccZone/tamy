#include "core-Renderer/RenderingPipelineNode.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RenderingPipelineNode, ResourceObject );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_neighbors );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::addNeighbor( RenderingPipelineNode& node )
{
   std::vector< RenderingPipelineNode* >::iterator it = std::find( m_neighbors.begin(), m_neighbors.end(), &node );
   if ( it == m_neighbors.end() )
   {
      m_neighbors.push_back( &node );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::removeNeighbor( RenderingPipelineNode& node )
{
   std::vector< RenderingPipelineNode* >::iterator it = std::find( m_neighbors.begin(), m_neighbors.end(), &node );
   if ( it != m_neighbors.end() )
   {
      m_neighbors.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////
