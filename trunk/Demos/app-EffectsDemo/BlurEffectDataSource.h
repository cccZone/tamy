#pragma once

#include "core-Renderer\EffectDataSource.h"
#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\CyclicRenderingTarget.h"
#include "core-Renderer\ChangableTexture.h"
#include <vector>
#include <list>
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

class BlurEffectDataSource : public EffectDataSource
{
private:
   CyclicRenderingTarget& m_cyclicTarget;
   std::vector<ChangableTexture*> m_textures;
   std::vector<std::string> m_names;
   unsigned int m_switchTime;

public:
   BlurEffectDataSource(CyclicRenderingTarget& cyclicTarget)
      : m_cyclicTarget(cyclicTarget),
      m_switchTime(0)
   {
      ASSERT(cyclicTarget.getTargets().size() == 5, "Invalid number of textures");

      m_textures.push_back(new ChangableTexture("tex"));
      m_textures.push_back(new ChangableTexture("tex"));
      m_textures.push_back(new ChangableTexture("tex"));
      m_textures.push_back(new ChangableTexture("tex"));
      m_textures.push_back(new ChangableTexture("tex"));
      m_names.push_back("texturePrev3");
      m_names.push_back("texturePrev2");
      m_names.push_back("texturePrev1");
      m_names.push_back("texturePrev0");
      m_names.push_back("textureNow");
   }

   ~BlurEffectDataSource()
   {
      unsigned int count = m_textures.size();
      for (unsigned int i = 0; i < m_textures.size(); ++i)
      {
         delete m_textures[i];
      }
      m_textures.clear();
   }

   void setData(GraphicalEffect& effect)
   {
      effect.setTechnique("render");

      const std::list<TextureRenderingTarget*>& targets = m_cyclicTarget.getTargets();
      ASSERT(targets.size() == m_textures.size(), "Number of render targets changed");

      unsigned int textureIdx = 0;
      for (std::list<TextureRenderingTarget*>::const_iterator it = targets.begin();
           it != targets.end(); ++it, ++textureIdx)
      {
         TextureRenderingTarget* target = (*it);

         m_textures[textureIdx]->setImpl(target->getImpl());
         effect.set(m_names[textureIdx].c_str(), *(m_textures[textureIdx]));
      }

      ++m_switchTime;
      if (m_switchTime == 2)
      {
         m_cyclicTarget.switchBuffer();
         m_switchTime = 0;
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
