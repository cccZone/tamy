#pragma once

#include "core-Renderer\Renderer.h"


///////////////////////////////////////////////////////////////////////////////

class RendererStub : public Renderer
{
protected:
   void resetViewport(unsigned int width, unsigned int height) {}

   void renderingBegin() {}

   void renderingEnd() {}

   bool isGraphicsSystemReady() const {return true;}

   void attemptToRecoverGraphicsSystem() {}

   void cleanAllTargets(unsigned int count) {}
};

///////////////////////////////////////////////////////////////////////////////
