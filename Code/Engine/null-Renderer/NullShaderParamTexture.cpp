#include "core-Renderer/ShaderParam.h"


///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamTexture< EffectShader >::setParam( Renderer& renderer, void* shaderPtr )
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamTexture< PixelShader >::setParam( Renderer& renderer, void* shaderPtr )
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamTexture< VertexShader >::setParam( Renderer& renderer, void* shaderPtr )
{
}

///////////////////////////////////////////////////////////////////////////////
