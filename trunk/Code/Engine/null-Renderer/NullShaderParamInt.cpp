#include "core-Renderer/ShaderParam.h"


///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamInt< EffectShader >::setParam( Renderer& renderer, void* shaderPtr )
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamInt< PixelShader >::setParam( Renderer& renderer, void* shaderPtr )
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void ShaderParamInt< VertexShader >::setParam( Renderer& renderer, void* shaderPtr )
{
}

///////////////////////////////////////////////////////////////////////////////