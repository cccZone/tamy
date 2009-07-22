#pragma once

#include "core-Renderer\PostProcessEffectNode.h"
#include "RendererImplementationMock.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace PostProcessMechanismTests
{
  
   class PostProcessEffectNodeMock : public PostProcessEffectNode
   {
   private:
      static RegularTests::RendererImplementationMock s_renderer;
      std::vector<std::string>& m_seqLog;

   public:
      PostProcessEffectNodeMock(const std::string& name,
                            RenderingTechnique& technique,
                            std::vector<std::string>& seqLog)
            : PostProcessEffectNode(name, technique, s_renderer),
            m_seqLog(seqLog)
      {}

      void render()
      {
         m_seqLog.push_back(getName() + " - render");
      }

   protected:
      void recreateGeometry(unsigned int width, unsigned int height)
      {
         m_seqLog.push_back(getName() + " - recreateGeometry");
      }
   };
}

///////////////////////////////////////////////////////////////////////////////
