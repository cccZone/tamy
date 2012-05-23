#include "dx9-Renderer\DX9VertexShader.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core-Renderer\ShaderTexture.h"
#include "core\File.h"
#include "dx9-Renderer\DXErrorParser.h"
#include <stdexcept>
#include "dx9-Renderer/DX9ShaderIncludeLoader.h"


///////////////////////////////////////////////////////////////////////////////

void RCBindVertexShader::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   DX9VertexShader* dxShader = dxRenderer.getVertexShader( m_shader );
   if ( dxShader )
   {
      // set the shader parameters
      setParams( renderer, dxShader );
      dxShader->beginRendering();
   }
}

///////////////////////////////////////////////////////////////////////////////

void RCUnbindVertexShader::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   DX9VertexShader* dxShader = dxRenderer.getVertexShader( m_shader );
   if ( dxShader )
   {
      dxShader->endRendering();
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DX9VertexShader::DX9VertexShader( const DX9Renderer& renderer, const VertexShader& shader )
   : m_shader( shader )
   , m_renderer( renderer )
   , m_d3Device( &renderer.getD3Device() )
   , m_dxVertexShader( NULL )
   , m_shaderConstants( NULL )
   , m_vertexDecl( NULL )
{
   initialize();
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

void DX9VertexShader::initialize()
{
   // load the effect
   const std::string& shaderContents = m_shader.getScript();
   const char* shaderProfile = D3DXGetVertexShaderProfile( m_d3Device );

   DWORD flags = 0;
#ifdef _DEBUG
   flags = D3DXSHADER_DEBUG;
#endif

   ID3DXBuffer* shaderBuf = NULL;
   ID3DXBuffer* errorsBuf = NULL;
   DX9ShaderIncludeLoader includesLoader;
   HRESULT res = D3DXCompileShader(
      shaderContents.c_str(), 
      shaderContents.length(),
      NULL,                            // defines
      &includesLoader,
      m_shader.getEntryFunctionName().c_str(),
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
         std::string errMsg = std::string( "Shader compilation error: " ) + compilationErrors;
         ASSERT_MSG( false, errMsg.c_str() );
         return;
      }
      else
      {
         std::string errMsg = translateDxError( "Error while compiling a shader", res );
         ASSERT_MSG( false, errMsg.c_str() );
         return;
      }
   }

   res = m_d3Device->CreateVertexShader( ( const DWORD* )shaderBuf->GetBufferPointer(), &m_dxVertexShader );
   shaderBuf->Release();
   if ( FAILED(res) )
   {
      std::string errMsg = translateDxError( "Error while creating a shader", res );
      ASSERT_MSG( false, errMsg.c_str() );
      return;
   }

   // create the vertex declaration
   // <renderer.todo> right now VertexDescriptor is an exact replica of D3DVERTEXELEMENT9 - but this needs
   // to change in  order to support OpenGL, DX11 etc.
   res = m_d3Device->CreateVertexDeclaration( (const D3DVERTEXELEMENT9 *)m_shader.getVerexDescription(), &m_vertexDecl );
   if ( FAILED( res ) )
   {
      ASSERT_MSG( false, "Can't create a vertex declaration" );
      return;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setBool( const char* paramName, bool val )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetBool( m_d3Device, hConstant, val );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setInt( const char* paramName, int val )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetInt( m_d3Device, hConstant, val );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setIntArray( const char* paramName, int* valsArr, unsigned int size )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      HRESULT res = m_shaderConstants->SetIntArray( m_d3Device, hConstant, valsArr, size );
      ASSERT( SUCCEEDED( res ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setFloat( const char* paramName, float val )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetFloat( m_d3Device, hConstant, val );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setFloatArray( const char* paramName, float* valsArr, unsigned int size )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      HRESULT res = m_shaderConstants->SetFloatArray( m_d3Device, hConstant, valsArr, size );
      ASSERT( SUCCEEDED( res ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setMtx( const char* paramName, const D3DXMATRIX& matrix )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetMatrix( m_d3Device, hConstant, &matrix );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setMtxArray( const char* paramName, const D3DXMATRIX* matrices, unsigned int size )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      HRESULT res = m_shaderConstants->SetMatrixArray( m_d3Device, hConstant, matrices, size );
      ASSERT( SUCCEEDED( res ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setVec4( const char* paramName, const D3DXVECTOR4& vec )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      m_shaderConstants->SetVector( m_d3Device, hConstant, &vec );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setVec4Array( const char* paramName, const D3DXVECTOR4* vecArr, unsigned int size )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      HRESULT res = m_shaderConstants->SetVectorArray( m_d3Device, hConstant, vecArr, size );
      ASSERT_MSG( SUCCEEDED( res ), translateDxError( "setVec4Array", res ).c_str() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::setTexture( const char* paramName, IDirect3DTexture9* texture )
{
   if ( m_shaderConstants )
   {
      D3DXHANDLE hConstant = m_shaderConstants->GetConstantByName( NULL, paramName );
      UINT samplerIdx = m_shaderConstants->GetSamplerIndex( hConstant );
      
      m_d3Device->SetTexture( samplerIdx, texture );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::beginRendering()
{
   ASSERT( m_vertexDecl != NULL );
   m_d3Device->SetVertexDeclaration( m_vertexDecl );
   m_d3Device->SetVertexShader( m_dxVertexShader );
}

///////////////////////////////////////////////////////////////////////////////

void DX9VertexShader::endRendering()
{
   m_d3Device->SetVertexDeclaration( NULL );
   m_d3Device->SetVertexShader( NULL );
}

///////////////////////////////////////////////////////////////////////////////
