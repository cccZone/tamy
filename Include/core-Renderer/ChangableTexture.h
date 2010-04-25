#pragma once

#include "core-Renderer\Texture.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class allows for texture implementation changes
 * and assumes the implementation it's managed externaly
 */
class ChangableTexture : public Texture
{
private:
   TextureImpl* m_impl;

public:
   ChangableTexture(const std::string& name) : Texture(name) {}
   ~ChangableTexture() {m_impl = NULL;}

   TextureImpl& getImpl() {return *m_impl;}
   void setImpl(TextureImpl& impl) {m_impl = &impl;}
};

///////////////////////////////////////////////////////////////////////////////
