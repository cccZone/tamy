#include "core-Renderer/ShaderParam.h"
#include "dx9-Renderer/DX9Renderer.h"
#include "dx9-Renderer/DX9PixelShader.h"
#include "dx9-Renderer/DX9VertexShader.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamInt< EffectShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   ID3DXEffect* dxEffect = reinterpret_cast< ID3DXEffect* >( shaderPtr );
   dxEffect->SetInt( m_name.c_str(), m_val );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamInt< PixelShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   DX9PixelShader* dxPixelShader = reinterpret_cast< DX9PixelShader* >( shaderPtr );
   dxPixelShader->setInt( m_name.c_str(), m_val );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamInt< VertexShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   DX9VertexShader* dxVertexShader = reinterpret_cast< DX9VertexShader* >( shaderPtr );
   dxVertexShader->setInt( m_name.c_str(), m_val );
}

///////////////////////////////////////////////////////////////////////////////
