/// @file   dx9-Renderer/DX9VertexShader.h
/// @brief  a DirectX9 vertex shader implementation

#pragma once

#include "core-Renderer\VertexShader.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

class DX9VertexShader
{
private:
   const VertexShader&                          m_shader;
   const DX9Renderer&                           m_renderer;
   IDirect3DDevice9*                            m_d3Device;
   std::vector< IDirect3DVertexShader9* >       m_dxVertexShader;
   ID3DXConstantTable*                          m_shaderConstants;
   IDirect3DVertexDeclaration9*                 m_vertexDecl;

public:
   DX9VertexShader( const DX9Renderer& renderer, const VertexShader& shader );
   ~DX9VertexShader();

   // -------------------------------------------------------------------------
   // Parameters setting
   // -------------------------------------------------------------------------
   void setBool( const char* paramName, bool val );
   void setFloat( const char* paramName, float val );
   void setFloatArray( const char* paramName, float* valsArr, unsigned int size );
   void setInt( const char* paramName, int val );
   void setIntArray( const char* paramName, int* valsArr, unsigned int size );
   void setMtx( const char* paramName, const D3DXMATRIX& matrix );
   void setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int count );
   void setVec4( const char* paramName, const D3DXVECTOR4& vec );
   void setVec4Array( const char* paramName, const D3DXVECTOR4* vecArr, unsigned int size );
   void setTexture( const char* paramName, IDirect3DTexture9* texture );
   void beginRendering( uint techniqueIdx );
   void endRendering();

private:
   void initialize();
};

///////////////////////////////////////////////////////////////////////////////
