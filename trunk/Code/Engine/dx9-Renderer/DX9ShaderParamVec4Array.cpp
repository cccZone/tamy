#include "core-Renderer/ShaderParam.h"
#include "dx9-Renderer/DX9Renderer.h"
#include "dx9-Renderer/DX9PixelShader.h"
#include "dx9-Renderer/DX9VertexShader.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamVec4Array< EffectShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   ID3DXEffect* dxEffect = reinterpret_cast< ID3DXEffect* >( shaderPtr );
   dxEffect->SetVectorArray( m_name.c_str(), ( const D3DXVECTOR4* )m_val, m_size );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamVec4Array< PixelShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   DX9PixelShader* dxPixelShader = reinterpret_cast< DX9PixelShader* >( shaderPtr );
   dxPixelShader->setVec4Array( m_name.c_str(), ( const D3DXVECTOR4* )m_val, m_size );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamVec4Array< VertexShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   DX9VertexShader* dxVertexShader = reinterpret_cast< DX9VertexShader* >( shaderPtr );
   dxVertexShader->setVec4Array( m_name.c_str(), ( const D3DXVECTOR4* )m_val, m_size );
}

///////////////////////////////////////////////////////////////////////////////
