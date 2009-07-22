#pragma once

#include "core-Renderer\SkyBox.h"
#include "StageTextureRendererStub.h"
#include "RenderingTargetsPolicyMock.h"


///////////////////////////////////////////////////////////////////////////////

class SkyBoxMock : public SkyBox
{
private:
   static StageTextureRendererStub s_stageTextureRenderer;
   static RegularTests::RenderingTargetsPolicyMock s_policy;

   bool m_wasRenderingStarted;
   bool m_wasRenderingEnded;

public:
   SkyBoxMock() 
      : SkyBox(s_stageTextureRenderer, s_policy), 
      m_wasRenderingStarted(false), 
      m_wasRenderingEnded(false) {}

   bool wasRendered() const {return (m_wasRenderingStarted && m_wasRenderingEnded);}

protected:
   void startRendering() {m_wasRenderingStarted = true;}

   void renderSide(SkyBoxSides sideIdx) {}

   void endRendering() {m_wasRenderingEnded = true;}
};

///////////////////////////////////////////////////////////////////////////////
