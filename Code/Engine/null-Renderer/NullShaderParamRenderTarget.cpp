#include "core-Renderer/ShaderParam.h"


///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamRenderTarget< EffectShader >::setParam( Renderer& renderer, void* shaderPtr )
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamRenderTarget< PixelShader >::setParam( Renderer& renderer, void* shaderPtr )
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamRenderTarget< VertexShader >::setParam( Renderer& renderer, void* shaderPtr )
{
}

///////////////////////////////////////////////////////////////////////////////
