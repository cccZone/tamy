#include "SceneQueryEffect.h"
#include "core-Renderer.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

SceneQueryEffect::SceneQueryEffect( ResourcesManager& rm )
   : m_renderedPtr( 0, 0, 0, 0 )
{
   // load the shader
   static const char* shaderName = "Editor/Shaders/SceneQueryEffect.psh";
   m_shader = dynamic_cast< PixelShader* >( rm.findResource( "SceneQueryEffect" ) );
   if ( !m_shader )
   {
      m_shader = new PixelShader( "SceneQueryEffect" );
      m_shader->loadFromFile( rm.getFilesystem(), shaderName );
      rm.addResource( m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueryEffect::render( Geometry& geometry )
{
   m_shader->setVec4( "g_ptr", m_renderedPtr );
   m_shader->beginRendering();
   geometry.render();
   m_shader->endRendering();
}

///////////////////////////////////////////////////////////////////////////////
