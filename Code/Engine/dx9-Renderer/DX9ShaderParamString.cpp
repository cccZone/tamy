#include "core-Renderer/ShaderParam.h"
#include "dx9-Renderer/DX9Renderer.h"
#include "dx9-Renderer/DX9PixelShader.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamString< EffectShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   ID3DXEffect* dxEffect = reinterpret_cast< ID3DXEffect* >( shaderPtr );
   dxEffect->SetString( m_name.c_str(), m_val.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamString< PixelShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   // can't do that
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamString< VertexShader >::setParam( Renderer& renderer, void* shaderPtr )
{
   // can't do that
}

///////////////////////////////////////////////////////////////////////////////
