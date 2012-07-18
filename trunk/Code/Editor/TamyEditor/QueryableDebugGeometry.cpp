#include "QueryableDebugGeometry.h"
#include "core-Renderer.h"
#include "core.h"
#include "QueryRenderingPass.h"
#include "DebugGeometry.h"


///////////////////////////////////////////////////////////////////////////////

QueryableDebugGeometry::QueryableDebugGeometry( DebugGeometry& geometry )
   : m_geometry( geometry )
{

}

///////////////////////////////////////////////////////////////////////////////

void QueryableDebugGeometry::render( Renderer& renderer, PixelShader& shader )
{
   if ( !m_geometry.hasMesh() )
   {
      return;
   }

   RCBindPixelShader* effectComm = new ( renderer() ) RCBindPixelShader( shader );

   Vector ptrVal;
   QueryRenderingPass::ptrToVec( m_geometry.getRepresentedObjectPtr(), ptrVal );
   effectComm->setVec4( "g_ptr", ptrVal );

   m_geometry.render( renderer );

   new ( renderer() ) RCUnbindPixelShader( shader );
}

///////////////////////////////////////////////////////////////////////////////
