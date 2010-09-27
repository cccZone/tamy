#include "SceneQueryEffect.h"
#include "core-Renderer.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

SceneQueryEffect::SceneQueryEffect( ResourcesManager& rm )
   : m_renderedPtr( 0, 0, 0, 0 )
{
   // load the shader
   static const char* shaderName = "Editor/Shaders/SceneQueryEffect.psh";
   m_shader = dynamic_cast< Shader* >( rm.findResource( shaderName ) );
   if ( !m_shader )
   {
      m_shader = new Shader( shaderName, SHT_PIXEL_SHADER );
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
