#pragma once

#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\Texture.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This rendering target cann be used as a texture
 */
class TextureRenderingTarget : public Texture,
                               public RenderingTarget
{
public:
   virtual ~TextureRenderingTarget() {}

protected:
   TextureRenderingTarget(const std::string& name) : Texture(name) {}
};

///////////////////////////////////////////////////////////////////////////////
