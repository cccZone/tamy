#include "RPBDepthNormals.h"
#include "core-Renderer/RPDepthNormalsNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBDepthNormals, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPDepthNormalsNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBDepthNormals::RPBDepthNormals( RPDepthNormalsNode& node )
   : RenderingPipelineBlock( node )
   , m_nodePtr( &node )
{
   setCaption( "Depth & normals" );
}

///////////////////////////////////////////////////////////////////////////////

RPBDepthNormals::~RPBDepthNormals()
{
   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPBDepthNormals::initialize( RenderingPipeline& parentResource )
{
   m_node = new TResourceHandle< RPDepthNormalsNode >( *m_nodePtr );
}

///////////////////////////////////////////////////////////////////////////////

Object& RPBDepthNormals::getNode() 
{ 
   return m_node->get();
}

///////////////////////////////////////////////////////////////////////////////
