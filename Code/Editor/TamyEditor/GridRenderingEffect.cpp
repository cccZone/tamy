#include "GridRenderingEffect.h"
#include "core-Renderer.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   const char* shaderCode = ""
   "float4x4 g_mWorldViewProj;"
   ""
   "struct VS_OUTPUT"
   "{"
   "   float4 pos   : POSITION;"
   "};"
   ""
   "VS_OUTPUT vertexShader(float4 inPos : POSITION)"
   "{"
   "   VS_OUTPUT output = (VS_OUTPUT)0;"
   ""
   "   output.pos = mul(inPos, g_mWorldViewProj);"
   ""
   "   return output;"
   "}"
   ""
   "struct PS_OUTPUT"
   "{"
   "   float4 color : COLOR0;   "
   "};"
   ""
   "PS_OUTPUT pixelShader(VS_OUTPUT vsIn)"
   "{"
   "   PS_OUTPUT output = (PS_OUTPUT)0;"
   ""
   "   output.color.ra = 1;"
   "   output.color.gb = 0;"
   ""
   "   return output;"
   "}"
   ""
   "technique tec0"
   "{"
   "   pass P0"
   "   {"
   "      Lighting = FALSE;"
   "      ZENABLE = true;"
   ""
   "      VertexShader = compile vs_2_0 vertexShader();"
   "      PixelShader  = compile ps_2_0 pixelShader();"
   "   }"
   "}";
} // anonymous

///////////////////////////////////////////////////////////////////////////////

GridRenderingEffect::GridRenderingEffect( GeometryResource* geometry, ResourcesManager& rm, Camera& camera )
: m_geometry( geometry )
, m_camera( camera )
, m_shader( NULL )
{
   // load the shader
   static const char* shaderName = "GridRenderingEffectShader";
   m_shader = dynamic_cast< EffectShader* >( rm.findResource( shaderName ) );
   if ( !m_shader )
   {
      m_shader = new EffectShader( shaderName, shaderCode );
      rm.addResource( m_shader );
   }
} 

///////////////////////////////////////////////////////////////////////////////

GridRenderingEffect::~GridRenderingEffect()
{
   m_shader = NULL;

   delete m_geometry;
   m_geometry = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::render()
{
   if ( !m_shader || !m_geometry )
   {
      return;
   }

   D3DXMATRIX worldViewProjMtx = m_camera.getViewMtx() * m_camera.getProjectionMtx();
   m_shader->setMtx( "g_mWorldViewProj", worldViewProjMtx );
   m_shader->setTechnique("tec0");
   m_shader->render( *m_geometry );
}

///////////////////////////////////////////////////////////////////////////////
