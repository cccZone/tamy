#include "RPBPostProcess.h"
#include "core-Renderer/RPPostProcessNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBPostProcess, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPPostProcessNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBPostProcess::RPBPostProcess( RPPostProcessNode& node )
   : m_nodePtr( &node )
{
   setCaption( "PostProcess" );

   addSocket().initialize( GBSP_LEFT, "Input" );
   addSocket().initialize( GBSP_RIGHT, "Output" );
}

///////////////////////////////////////////////////////////////////////////////

RPBPostProcess::~RPBPostProcess()
{
   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPBPostProcess::initialize( RenderingPipeline& parentResource )
{
   m_node = new TResourceHandle< RPPostProcessNode >( *m_nodePtr );
}

///////////////////////////////////////////////////////////////////////////////

Object& RPBPostProcess::getNode() 
{ 
   return m_node->get();
}

///////////////////////////////////////////////////////////////////////////////
