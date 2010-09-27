#pragma once

#include "core-Renderer\Shader.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

class DX9Shader : public ShaderImpl
{
private:
   Shader&                       m_shader;
   DX9Renderer*                  m_renderer;
   IDirect3DDevice9*             m_d3Device;
   IDirect3DVertexShader9*       m_dxVertexShader;
   IDirect3DPixelShader9*        m_dxPixelShader;
   ID3DXConstantTable*           m_shaderConstants;
   IDirect3DVertexDeclaration9*  m_vertexDecl;

public:
   DX9Shader( Shader& shader );
   ~DX9Shader();

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
