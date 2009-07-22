#pragma once

#include "core-Renderer\EffectDataSource.h"
#include "core-Renderer\GraphicalEffect.h"


///////////////////////////////////////////////////////////////////////////////

class Texture;

///////////////////////////////////////////////////////////////////////////////

class WavyImageDataSource : public EffectDataSource
{
private:
   Texture& m_inputTexture;

public:
   WavyImageDataSource(Texture& inputTexture)
      : m_inputTexture(inputTexture)
   {}

   void setData(GraphicalEffect& effect)
   {
      effect.setTechnique("render");
      effect.set("inputTexture", m_inputTexture);
   }
};

///////////////////////////////////////////////////////////////////////////////
