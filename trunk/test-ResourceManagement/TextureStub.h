#pragma once

#include "core-Renderer\Texture.h"


//////////////////////////////////////////////////////////////////////////////

class TextureStub : public Texture
{
public:
   TextureStub(const std::string& name = "") : Texture(name) {}

   void setForRendering(unsigned char stageIdx) {}
};

//////////////////////////////////////////////////////////////////////////////
