#include "core-Renderer/RenderableTexture.h"
#include "core-Renderer/Texture.h"
#include "core/IDString.h"


///////////////////////////////////////////////////////////////////////////////

RenderableTexture::RenderableTexture()
    : m_texture( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderableTexture::setTexture( Texture* texture )
{
    m_texture = texture;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int RenderableTexture::getWidth() const
{
    return m_texture ? m_texture->getWidth() : 0;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int RenderableTexture::getHeight() const
{
    return m_texture ? m_texture->getHeight() : 0;
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< EffectShader >* RenderableTexture::createEffectShaderTextureSetter( MemoryPoolAllocator& allocator, const IDString& paramName )
{
    return new ( allocator ) ShaderParamTexture< EffectShader >( paramName.getId(), m_texture );
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< PixelShader >* RenderableTexture::createPixelShaderTextureSetter( MemoryPoolAllocator& allocator, const IDString& paramName )
{
    return new ( allocator ) ShaderParamTexture< PixelShader >( paramName.getId(), m_texture );
}

///////////////////////////////////////////////////////////////////////////////

ShaderParam< VertexShader >* RenderableTexture::createVertexShaderTextureSetter( MemoryPoolAllocator& allocator, const IDString& paramName )
{
    return new ( allocator ) ShaderParamTexture< VertexShader >( paramName.getId(), m_texture );
}

///////////////////////////////////////////////////////////////////////////////
