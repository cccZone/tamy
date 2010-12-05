#include "RPBPostProcess.h"
#include "core-Renderer/RPPostProcessNode.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBPostProcess, GraphBlock )
   PROPERTY_EDIT( "node", RPPostProcessNode*, m_node )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPBPostProcess::RPBPostProcess( RPPostProcessNode& node )
: m_node( &node )
{
   setCaption( "PostProcess" );

   addSocket().initialize( GBSP_LEFT, "Input" );
   addSocket().initialize( GBSP_RIGHT, "Output" );
}

///////////////////////////////////////////////////////////////////////////////

Object* RPBPostProcess::getNode() const 
{ 
   return m_node; 
}

///////////////////////////////////////////////////////////////////////////////
