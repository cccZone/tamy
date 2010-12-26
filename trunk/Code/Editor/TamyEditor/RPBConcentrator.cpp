#include "RPBConcentrator.h"
#include "core-Renderer/RPConcentratorNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBConcentrator, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPConcentratorNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBConcentrator::RPBConcentrator( RPConcentratorNode& node )
   : m_nodePtr( &node )
{
   setCaption( "Concentrator" );

   addSocket().initialize( GBSP_LEFT, "Input" );
   addSocket().initialize( GBSP_RIGHT, "Output" );
}

///////////////////////////////////////////////////////////////////////////////

RPBConcentrator::~RPBConcentrator()
{
   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPBConcentrator::initialize( RenderingPipeline& parentResource )
{
   m_node = new TResourceHandle< RPConcentratorNode >( *m_nodePtr );
}

///////////////////////////////////////////////////////////////////////////////

Object& RPBConcentrator::getNode() 
{ 
   return m_node->get();
}

///////////////////////////////////////////////////////////////////////////////
