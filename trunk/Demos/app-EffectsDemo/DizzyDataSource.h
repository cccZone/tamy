#pragma once

#include "core-Renderer\EffectDataSource.h"
#include "core-Renderer\GraphicalEffect.h"


///////////////////////////////////////////////////////////////////////////////

class Texture;

///////////////////////////////////////////////////////////////////////////////

class DizzyDataSource : public EffectDataSource
{
private:
   Texture& m_inputTexture;
   float m_time;

public:
   DizzyDataSource(Texture& inputTexture)
      : m_inputTexture(inputTexture),
      m_time(0)
   {}

   void setData(GraphicalEffect& effect)
   {
      effect.setTechnique("render");
      effect.set("inputTexture", m_inputTexture);
      effect.set("time", m_time);

      m_time += 0.001f;
   }
};

///////////////////////////////////////////////////////////////////////////////
