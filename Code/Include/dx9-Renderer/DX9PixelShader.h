/// @file   dx9-Renderer/DX9PixelShader.h
/// @brief  a DirectX9 pixel shader implementation

#pragma once

#include "core-Renderer\PixelShader.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

class DX9PixelShader : public PixelShaderImpl
{
private:
   PixelShader&                  m_shader;
   DX9Renderer*                  m_renderer;
   IDirect3DDevice9*             m_d3Device;
   IDirect3DPixelShader9*        m_dxPixelShader;
   ID3DXConstantTable*           m_shaderConstants;

public:
   DX9PixelShader( PixelShader& shader );
   ~DX9PixelShader();

   // -------------------------------------------------------------------------
   // RendererObjectImpl implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );

   // -------------------------------------------------------------------------
   // ShaderImpl implementation
   // -------------------------------------------------------------------------
   void setBool( const char* paramName, bool val );
   void setMtx( const char* paramName, const D3DXMATRIX& matrix );
   void setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int count );
   void setVec4( const char* paramName, const D3DXVECTOR4& vec );
   void setTexture( const char* paramName, ShaderTexture& val );
   void beginRendering();
   void endRendering();
};

///////////////////////////////////////////////////////////////////////////////
