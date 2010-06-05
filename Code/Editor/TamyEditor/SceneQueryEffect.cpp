#include "SceneQueryEffect.h"
#include "core-Renderer.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

SceneQueryEffect::SceneQueryEffect( ResourcesManager& rm, Camera& camera )
: m_camera( camera )
, m_renderedPtr( 0, 0, 0, 0 )
{
   D3DXMatrixIdentity( &m_objMtx );

   // load the shader
   Shader* shader = dynamic_cast< Shader* >( rm.findResource( "SceneQueryEffect" ) );
   if ( !shader )
   {
      // load the shader script
      File* shaderFile = rm.getFilesystem().open( "Editor/Shaders/SceneQueryEffect.fx" );
      StreamBuffer< char > shaderFileBuf( *shaderFile );
      shader = new Shader( "SceneQueryEffect", shaderFileBuf.getBuffer() );
      rm.addResource( shader );
      delete shaderFile;
   }
   initialize( *shader );
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueryEffect::onBeginRendering()
{
   D3DXMATRIX worldViewProjMtx = m_objMtx * m_camera.getViewMtx() * m_camera.getProjectionMtx();
   shader().setMtx( "g_mWorldViewProj", worldViewProjMtx );

   shader().setVec4( "g_ptr", m_renderedPtr );

   shader().setTechnique("tec0");
}

///////////////////////////////////////////////////////////////////////////////
