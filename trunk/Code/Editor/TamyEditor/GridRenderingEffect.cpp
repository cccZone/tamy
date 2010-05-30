#include "GridRenderingEffect.h"
#include "core-Renderer.h"
#include "core\Node.h"


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

GridRenderingEffect::GridRenderingEffect( Renderer& renderer )
: m_camera(NULL)
, m_renderedNode(NULL)
, m_shader( NULL )
{
   m_shader = new Shader( "GridRenderingEffect", shaderCode );
   renderer.implement< Shader >( *m_shader );
   initialize( *m_shader );
} 

///////////////////////////////////////////////////////////////////////////////

GridRenderingEffect::~GridRenderingEffect()
{
   delete m_shader;
   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onBeginRendering()
{
   if ((m_camera == NULL) || (m_renderedNode == NULL))
   {
      return;
   }

   D3DXMATRIX worldViewProjMtx = m_renderedNode->getGlobalMtx() 
      * m_camera->getViewMtx() 
      * m_camera->getProjectionMtx();
   shader().setMtx("g_mWorldViewProj", worldViewProjMtx);

   shader().setTechnique("tec0");
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onAttached(Entity& parent) 
{
   m_renderedNode = dynamic_cast<Node*> (&parent);
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onDetached(Entity& parent) 
{
   m_renderedNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onAttached(Model& hostModel) 
{
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onDetached(Model& hostModel) 
{
   m_camera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GridRenderingEffect::onComponentAdded( Component< Model >& component ) 
{
   CameraComponent* comp = dynamic_cast< CameraComponent* >( &component );
   if ( comp )
   {
      m_camera = &comp->getCamera();
   }
}

///////////////////////////////////////////////////////////////////////////////
