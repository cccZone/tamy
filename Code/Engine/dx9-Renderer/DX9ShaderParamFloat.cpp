#include "core-Renderer/ShaderParam.h"
#include "dx9-Renderer/DX9Renderer.h"
#include "dx9-Renderer/DX9PixelShader.h"
#include "dx9-Renderer/DX9VertexShader.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamFloat< EffectShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   ID3DXEffect* dxEffect = reinterpret_cast< ID3DXEffect* >( shaderPtr );
   dxEffect->SetFloat( m_name.c_str(), m_val );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamFloat< PixelShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   DX9PixelShader* dxPixelShader = reinterpret_cast< DX9PixelShader* >( shaderPtr );
   dxPixelShader->setFloat( m_name.c_str(), m_val );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamFloat< VertexShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   DX9VertexShader* dxVertexShader = reinterpret_cast< DX9VertexShader* >( shaderPtr );
   dxVertexShader->setFloat( m_name.c_str(), m_val );
}

///////////////////////////////////////////////////////////////////////////////
