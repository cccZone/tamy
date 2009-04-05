#pragma once

#include "core-Renderer\SkyBox.h"


///////////////////////////////////////////////////////////////////////////////

class SkyBoxMock : public SkyBox
{
private:
   bool m_wasRenderingStarted;
   bool m_wasRenderingEnded;

public:
   SkyBoxMock() : m_wasRenderingStarted(false), m_wasRenderingEnded(false) {}
   bool wasRendered() const {return (m_wasRenderingStarted && m_wasRenderingEnded);}

protected:
   void startRendering() {m_wasRenderingStarted = true;}

   void renderSide(SkyBoxSides sideIdx) {}

   void endRendering() {m_wasRenderingEnded = true;}
};

///////////////////////////////////////////////////////////////////////////////
