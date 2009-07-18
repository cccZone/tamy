#pragma once

#include "core-Renderer\RenderingTarget.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingTargetMock : public RenderingTarget
{
private:
   int m_renderIdx;

public:
   RenderingTargetMock() 
      : m_renderIdx(-1)
   {}

   void use(unsigned char idx) { m_renderIdx = idx;}

   int getRenderingIdx() 
   {
      int result = m_renderIdx;
      m_renderIdx = -1;
      return result;
   }
};

///////////////////////////////////////////////////////////////////////////////
