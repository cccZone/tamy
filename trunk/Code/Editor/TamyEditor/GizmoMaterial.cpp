#include "GizmoMaterial.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/BasicRenderCommands.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(GizmoMaterial)
   PARENT( ReflectionObject )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

GizmoMaterial::GizmoMaterial( byte axisIdx, const Color& color )
   : m_axisIdx( axisIdx )
   , m_color( color )
{
}

///////////////////////////////////////////////////////////////////////////////

void GizmoMaterial::onPreRender( Renderer& renderer ) const
{
   new ( renderer() ) RCEnableLighting( false );
   new ( renderer() ) RCSetFillMode( GFM_Solid );
   new ( renderer() ) RCSetColorSource( MC_Ambient, SVC_Diffuse );
   new ( renderer() ) RCSetColorSource( MC_Diffuse, SVC_Diffuse );

   // disable Z-buffer testing and writing
   new ( renderer() ) RCSetZBuffer( false, false );
}

///////////////////////////////////////////////////////////////////////////////

void GizmoMaterial::onPostRender( Renderer& renderer ) const
{
   // restore Z-buffer testing and writing
   new ( renderer() ) RCSetZBuffer( true, true );
}

///////////////////////////////////////////////////////////////////////////////
