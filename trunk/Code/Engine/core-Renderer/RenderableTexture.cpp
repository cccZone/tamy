#include "core-Renderer/RenderableTexture.h"
#include "core-Renderer/Texture.h"


RenderableTexture::RenderableTexture()
    : m_texture( NULL )
{
}

void RenderableTexture::setTexture( Texture* texture )
{
    m_texture = texture;
}

unsigned int RenderableTexture::getWidth() const
{
    return m_texture ? m_texture->getWidth() : 0;
}

unsigned int RenderableTexture::getHeight() const
{
    return m_texture ? m_texture->getHeight() : 0;
}

ShaderParam< EffectShader >* RenderableTexture::createEffectShaderTextureSetter( const std::string& paramName )
{
    return m_texture ? new ShaderParamTexture< EffectShader >( paramName, *m_texture ) : NULL;
}

ShaderParam< PixelShader >* RenderableTexture::createPixelShaderTextureSetter( const std::string& paramName )
{
    return m_texture ? new ShaderParamTexture< PixelShader >( paramName, *m_texture ) : NULL;
}

ShaderParam< VertexShader >* RenderableTexture::createVertexShaderTextureSetter( const std::string& paramName )
{
    return m_texture ? new ShaderParamTexture< VertexShader >( paramName, *m_texture ) : NULL;
}
