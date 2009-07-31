#pragma once

#include "core-Renderer\PostProcessEffectRenderable.h"
#include "RendererImplementationMock.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace PostProcessMechanismTests
{
  
   class PostProcessEffectRenderableMock : public PostProcessEffectRenderable
   {
   private:
      std::string m_name;
      static RegularTests::RendererImplementationMock s_renderer;
      std::vector<std::string>& m_seqLog;

   public:
      PostProcessEffectRenderableMock(const std::string& name,
                                std::vector<std::string>& seqLog)
            : PostProcessEffectRenderable(s_renderer),
            m_name(name),
            m_seqLog(seqLog)
      {}

      void render()
      {
         m_seqLog.push_back(m_name + " - render");
      }

   protected:
      void recreateGeometry(unsigned int width, unsigned int height)
      {
         m_seqLog.push_back(m_name + " - recreateGeometry");
      }
   };
}

///////////////////////////////////////////////////////////////////////////////
