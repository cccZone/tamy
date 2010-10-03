#include "dx9-Renderer\DX9PixelShader.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core-Renderer\ShaderTexture.h"
#include "core\File.h"
#include "dx9-Renderer\DXErrorParser.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

DX9PixelShader::DX9PixelShader( PixelShader& shader )
   : m_shader( shader )
   , m_renderer( NULL )
   , m_d3Device( NULL )
   , m_dxPixelShader( NULL )
   , m_shaderConstants( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

DX9PixelShader::~DX9PixelShader()
{
   m_d3Device = NULL;

   if ( m_dxPixelShader != NULL )
   {
      m_dxPixelShader->Release();
      m_dxPixelShader = NULL;
   }

   if ( m_shaderConstants )
   {
      m_shaderConstants->Release();
      m_shaderConstants = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9PixelShader::initialize( Renderer& renderer )
{
   m_renderer = dynamic_cast< DX9Renderer* >( &renderer );
   if ( m_renderer == NULL )
   {
      throw std::runtime_error( "This implementation can work only with DX9Renderer" );
   }

   if ( m_dxPixelShader != NULL )
   {
      m_dxPixelShader->Release();
      m_dxPixelShader = NULL;
   }

   if ( m_shaderConstants )
   {
      m_shaderConstants->Release();
      m_shaderConstants = NULL;
   }

   // load the effect
   const std::string& shaderContents = m_shader.getScript();
   m_d3Device = &m_renderer->getD3Device();

   if ( shaderContents.empty() )
   {
      return;
   }

   const char* shaderProfile = D3DXGetPixelShaderProfile( m_d3Device );

   DWORD flags = 0;
#ifdef _DEBUG
   flags = D3DXSHADER_DEBUG;
#endif

   ID3DXBuffer* shaderBuf = NULL;
   ID3DXBuffer* errorsBuf = NULL;
   HRESULT res = D3DXCompileShader(
      shaderContents.c_str(), 
      shaderContents.length(),
      NULL,                            // defines
      NULL,                            // includes
      "main",                          // entry function
      shaderProfile, 
      flags,
      &shaderBuf, 
      &errorsBuf,
      &m_shaderConstants );

   if ( FAILED(res) || shaderBuf == NULL )
   {
      if ( errorsBuf != NULL )
      {
         std::string compilationErrors = ( const char* )errorsBuf->GetBufferPointer();
         errorsBuf->Release();
         throw std::runtime_error( std::string( "Shader compilation error: " ) + compilationErrors );
      }
      else
      {
         std::string errMsg = translateDxError( "Error while compiling a shader", res );
         throw std::runtime_error( errMsg );
      }
   }

   res = m_d3Device->CreatePixelShader( ( const DWORD* )shaderBuf->GetBufferPointer(), &m_dxPixelShader );
   shaderBuf->Release();
   if ( FAILED(res) )
   {
      std::string errMsg = translateDxError( "Error while creating a shader", res );
      throw std::runtime_error( errMsg );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9PixelShader::setBool( const char* paramName, bool val )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetBool( m_d3Device, hConstant, val );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9PixelShader::setMtx( const char* paramName, const D3DXMATRIX& matrix )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetMatrix( m_d3Device, hConstant, &matrix );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9PixelShader::setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int size )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      HRESULT res = m_shaderConstants->SetMatrixArray( m_d3Device, hConstant, matrices, size );
      ASSERT( SUCCEEDED( res ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9PixelShader::setVec4( const char* paramName, const D3DXVECTOR4& vec )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetVector( m_d3Device, hConstant, &vec );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9PixelShader::setTexture( const char* paramName, ShaderTexture& val )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      UINT samplerIdx = m_shaderConstants->GetSamplerIndex( hConstant );

      const PixelShaderParams& params = m_shader.getParams();
      m_d3Device->SetSamplerState( samplerIdx, D3DSAMP_ADDRESSU, params.m_addressU );
      m_d3Device->SetSamplerState( samplerIdx, D3DSAMP_ADDRESSV, params.m_addressV );
      m_d3Device->SetSamplerState( samplerIdx, D3DSAMP_ADDRESSW, params.m_addressW );
      m_d3Device->SetSamplerState( samplerIdx, D3DSAMP_MINFILTER, params.m_minFilter );
      m_d3Device->SetSamplerState( samplerIdx, D3DSAMP_MAGFILTER, params.m_magFilter );
      m_d3Device->SetSamplerState( samplerIdx, D3DSAMP_MIPFILTER, params.m_mipFilter );

      IDirect3DTexture9* texture = reinterpret_cast< IDirect3DTexture9* >( val.getPlatformSpecific() );
      m_d3Device->SetTexture( samplerIdx, texture );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9PixelShader::beginRendering()
{
   if ( m_d3Device )
   {
      const PixelShaderParams& params = m_shader.getParams();
      m_d3Device->SetRenderState( D3DRS_CULLMODE, params.m_cullingMode );
      m_d3Device->SetRenderState( D3DRS_ZENABLE, params.m_useZBuffer );
      m_d3Device->SetRenderState( D3DRS_ZWRITEENABLE, params.m_writeToZBuffer );
      m_d3Device->SetPixelShader( m_dxPixelShader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9PixelShader::endRendering()
{
   if ( m_d3Device )
   {
      m_d3Device->SetPixelShader( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////
