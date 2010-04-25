#pragma once

#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\Texture.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This rendering target can be used as a texture
 */
class TextureRenderingTarget : public RenderingTarget,
                               public Texture
{
public:
   virtual ~TextureRenderingTarget() {}

   /** 
    * @derrived
    */
   virtual void use(unsigned char idx) = 0;

   /** 
    * @derrived
    */
   virtual TextureImpl& getImpl() = 0;

protected:
   TextureRenderingTarget(const std::string& name) : Texture(name) {}
};

///////////////////////////////////////////////////////////////////////////////
