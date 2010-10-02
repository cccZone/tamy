#include "dx9-Renderer\DX9VertexShader.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core-Renderer\ShaderTexture.h"
#include "core\File.h"
#include "dx9-Renderer\DXErrorParser.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

DX9VertexShader::DX9VertexShader( VertexShader& shader )
   : m_shader( shader )
   , m_renderer( NULL )
   , m_d3Device( NULL )
   , m_dxVertexShader( NULL )
   , m_shaderConstants( NULL )
   , m_vertexDecl( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

DX9VertexShader::~DX9VertexShader()
{
   m_d3Device = NULL;

   if ( m_dxVertexShader != NULL )
   {
      m_dxVertexShader->Release();
      m_dxVertexShader = NULL;
   }

   if ( m_shaderConstants )
   {
      m_shaderConstants->Release();
      m_shaderConstants = NULL;
   }

   if ( m_vertexDecl )
   {
      m_vertexDecl->Release();
      m_vertexDecl = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::initialize( Renderer& renderer )
{
   m_renderer = dynamic_cast< DX9Renderer* >( &renderer );
   if ( m_renderer == NULL )
   {
      throw std::runtime_error( "This implementation can work only with DX9Renderer" );
   }

   if ( m_dxVertexShader != NULL )
   {
      m_dxVertexShader->Release();
      m_dxVertexShader = NULL;
   }

   if ( m_shaderConstants )
   {
      m_shaderConstants->Release();
      m_shaderConstants = NULL;
   }

   // load the effect
   const std::string& shaderContents = m_shader.getScript();
   m_d3Device = &m_renderer->getD3Device();

   const char* shaderProfile = D3DXGetVertexShaderProfile( m_d3Device );

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

   res = m_d3Device->CreateVertexShader( ( const DWORD* )shaderBuf->GetBufferPointer(), &m_dxVertexShader );
   shaderBuf->Release();
   if ( FAILED(res) )
   {
      std::string errMsg = translateDxError( "Error while creating a shader", res );
      throw std::runtime_error( errMsg );
   }

   // create the vertex declaration
   res = m_d3Device->CreateVertexDeclaration( m_shader.getVerexDescription(), &m_vertexDecl );
   if ( FAILED( res ) )
   {
      throw std::logic_error("Can't create a vertex declaration");
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setBool( const char* paramName, bool val )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetBool( m_d3Device, hConstant, val );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setMtx( const char* paramName, const D3DXMATRIX& matrix )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetMatrix( m_d3Device, hConstant, &matrix );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int size )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      HRESULT res = m_shaderConstants->SetMatrixArray( m_d3Device, hConstant, matrices, size );
      ASSERT( SUCCEEDED( res ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setVec4( const char* paramName, const D3DXVECTOR4& vec )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetVector( m_d3Device, hConstant, &vec );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setTexture( const char* paramName, ShaderTexture& val )
{
   if ( m_shaderConstants && m_d3Device )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      UINT samplerIdx = m_shaderConstants->GetSamplerIndex( hConstant );
      
      IDirect3DTexture9* texture = reinterpret_cast< IDirect3DTexture9* >( val.getPlatformSpecific() );
      m_d3Device->SetTexture( samplerIdx, texture );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::beginRendering()
{
   if ( m_d3Device )
   {
      ASSERT( m_vertexDecl != NULL );
      m_d3Device->SetVertexDeclaration( m_vertexDecl );
      m_d3Device->SetVertexShader( m_dxVertexShader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::endRendering()
{
   if ( m_d3Device )
   {
      m_d3Device->SetVertexDeclaration( NULL );
      m_d3Device->SetVertexShader( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////
