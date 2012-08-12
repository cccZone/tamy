#include "SingleColorDebugMat.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(SingleColorDebugMat)
   PARENT( ReflectionObject )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

SingleColorDebugMat::SingleColorDebugMat( const Color& color )
   : m_color( color )
{
   FilePath gizmoShaderPath( "Editor/Shaders/SingleColorDebugMat.tpsh" );
   m_shader = ResourcesManager::getInstance().create< PixelShader >( gizmoShaderPath );
   ASSERT_MSG( m_shader != NULL, "SingleColorDebugMat shader doesn't exist" );
}

///////////////////////////////////////////////////////////////////////////////

void SingleColorDebugMat::onPreRender( Renderer& renderer ) const
{
   if ( m_shader )
   {
      RCBindPixelShader* effectComm = new ( renderer() ) RCBindPixelShader( *m_shader, renderer );
      effectComm->setVec4( "g_color", (const Vector&)m_color );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SingleColorDebugMat::onPostRender( Renderer& renderer ) const
{
   if ( m_shader )
   {
      new ( renderer() ) RCUnbindPixelShader( *m_shader, renderer );
   }
}

///////////////////////////////////////////////////////////////////////////////
