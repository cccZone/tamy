#include "core-Renderer/BasicRenderCommands.h"
#include "dx9-Renderer\DX9Renderer.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

void RCSetFixedMaterial::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   D3DMATERIAL9 mat;
   d3Device.GetMaterial( &mat );

   switch( m_materialComponent )
   {
   case MC_Ambient:
      {
         mat.Ambient = (const D3DCOLORVALUE&)m_color;
         break;
      }

   case MC_Diffuse:
      {
         mat.Diffuse = (const D3DCOLORVALUE&)m_color;
         break;
      }

   case MC_Specular:
      {
         mat.Specular = (const D3DCOLORVALUE&)m_color;
         break;
      }

   case MC_Emissive:
      {
         mat.Emissive = (const D3DCOLORVALUE&)m_color;
         break;
      }
   }
   d3Device.SetMaterial( &mat );
}

///////////////////////////////////////////////////////////////////////////////

void RCSetAmbientColor::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   DWORD colorVal = D3DCOLOR_COLORVALUE( m_color.r, m_color.g, m_color.b, m_color.a );
   d3Device.SetRenderState( D3DRS_AMBIENT, colorVal );
}

///////////////////////////////////////////////////////////////////////////////

void RCSetZBuffer::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.SetRenderState( D3DRS_ZENABLE, m_performTest );
   d3Device.SetRenderState( D3DRS_ZWRITEENABLE, m_enableWrite );
}

///////////////////////////////////////////////////////////////////////////////

void RCEnableLighting::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.SetRenderState( D3DRS_LIGHTING, m_enable );
}

///////////////////////////////////////////////////////////////////////////////

void RCSetFillMode::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.SetRenderState( D3DRS_FILLMODE, (D3DFILLMODE)m_fillMode );
}

///////////////////////////////////////////////////////////////////////////////

void RCSetColorSource::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   D3DRENDERSTATETYPE dxRenderState = D3DRS_AMBIENTMATERIALSOURCE;
   switch( m_materialColor )
   {
      case MC_Ambient: dxRenderState = D3DRS_AMBIENTMATERIALSOURCE; break;
      case MC_Diffuse: dxRenderState = D3DRS_DIFFUSEMATERIALSOURCE; break;
      case MC_Specular: dxRenderState = D3DRS_SPECULARMATERIALSOURCE; break;
      case MC_Emissive: dxRenderState = D3DRS_EMISSIVEMATERIALSOURCE; break;
   }

   DWORD stateVal = (D3DMATERIALCOLORSOURCE)m_sourceVertexColor;

   d3Device.SetRenderState( dxRenderState, stateVal );
}

///////////////////////////////////////////////////////////////////////////////

void RCSetVertexDeclaration::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.SetFVF( m_vertexDeclaration );
}

///////////////////////////////////////////////////////////////////////////////

void RCSetWorldMatrix::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.SetTransform( D3DTS_WORLD, ( const D3DXMATRIX* )&m_mtx );
}

///////////////////////////////////////////////////////////////////////////////

void RCSetCameraMatrices::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = dynamic_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.SetTransform( D3DTS_PROJECTION, ( const D3DXMATRIX* )&m_projectionMtx );
   d3Device.SetTransform( D3DTS_VIEW, ( const D3DXMATRIX* )&m_viewMtx );
}

///////////////////////////////////////////////////////////////////////////////

void RCBeginScene::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   dxRenderer.beginScene();
}

///////////////////////////////////////////////////////////////////////////////

void RCEndScene::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   dxRenderer.endScene();
}

///////////////////////////////////////////////////////////////////////////////

void RCClearDepthBuffer::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   d3Device.Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
}

///////////////////////////////////////////////////////////////////////////////

void RCStateTest::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   IDirect3DDevice9& d3Device = dxRenderer.getD3Device();

   DWORD res = 0;
   {
      d3Device.GetRenderState( D3DRS_ZENABLE, &res );
      ASSERT_MSG( res == m_zTest, "Z testing is not set correctly" );
   }

   {
      d3Device.GetRenderState( D3DRS_ZWRITEENABLE, &res );
      ASSERT_MSG( res == m_zWrite, "Z writing is not set correctly" );
   }
}

///////////////////////////////////////////////////////////////////////////////
