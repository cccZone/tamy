#include "GizmoMaterial.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(GizmoMaterial)
   PARENT( ReflectionObject )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

GizmoMaterial::GizmoMaterial( byte axisIdx, const Color& color, PixelShader& shader )
   : m_axisIdx( axisIdx )
   , m_color( color )
   , m_shader( shader )
{
}

///////////////////////////////////////////////////////////////////////////////

void GizmoMaterial::onPreRender( Renderer& renderer ) const
{   
   RCBindPixelShader* effectComm = new ( renderer() ) RCBindPixelShader( m_shader );
   effectComm->setVec4( "g_color", (const Vector&)m_color );
}

///////////////////////////////////////////////////////////////////////////////

void GizmoMaterial::onPostRender( Renderer& renderer ) const
{
   new ( renderer() ) RCUnbindPixelShader( m_shader );
}

///////////////////////////////////////////////////////////////////////////////
