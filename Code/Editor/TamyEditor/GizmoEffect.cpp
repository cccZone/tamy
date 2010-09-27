#include "GizmoEffect.h"
#include "core.h"
#include "core-Renderer.h"


///////////////////////////////////////////////////////////////////////////////

GizmoEffect::GizmoEffect( ResourcesManager& rm, Camera& camera, Node& renderedNode )
: m_camera( camera )
, m_renderedNode( renderedNode )
{
   const char* shaderFileName = "/Editor/Shaders/GizmoEffect.fx";
   m_effect = dynamic_cast< EffectShader* >( rm.findResource( shaderFileName ) );

   if ( !m_effect )
   {
      // create the shader 
      m_effect = new EffectShader( shaderFileName );
      rm.addResource( m_effect );
   }
} 

///////////////////////////////////////////////////////////////////////////////

void GizmoEffect::render( GeometryResource& geometry )
{
   D3DXMATRIX worldViewProjMtx = m_renderedNode.getGlobalMtx() * m_camera.getViewMtx() * m_camera.getProjectionMtx();
   m_effect->setMtx( "g_mWorldViewProj", worldViewProjMtx );
   m_effect->setTechnique("tec0");
   m_effect->render( geometry );
}

///////////////////////////////////////////////////////////////////////////////
