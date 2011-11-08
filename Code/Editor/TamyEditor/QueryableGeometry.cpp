#include "QueryableGeometry.h"
#include "core-Renderer.h"
#include "core.h"
#include "QueryRenderingPass.h"


///////////////////////////////////////////////////////////////////////////////

QueryableGeometry::QueryableGeometry( Geometry& geometry )
: m_geometry( geometry )
{

}

///////////////////////////////////////////////////////////////////////////////

void QueryableGeometry::render( Renderer& renderer, PixelShader& shader )
{
   if ( !m_geometry.hasGeometry() )
   {
      return;
   }

   RCBindPixelShader* effectComm = new ( renderer() ) RCBindPixelShader( shader );
   effectComm->setVec4( "g_ptr", QueryRenderingPass::ptrToVec( &m_geometry ) );

   m_geometry.render( renderer );

   new ( renderer() ) RCUnbindPixelShader( shader );
}

///////////////////////////////////////////////////////////////////////////////
