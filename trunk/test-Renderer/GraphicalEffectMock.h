#pragma once

#include "core-Renderer\GraphicalEffect.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEffectMock : public GraphicalEffect
{
private:
   bool m_wasRendered;

public:
   GraphicalEffectMock() 
         : m_wasRendered(false)
   {}

   void render()
   {
      m_wasRendered = true;
   }

   bool wasRendered() 
   {
      bool result = m_wasRendered;
      m_wasRendered = false;
      return result;
   }
};

///////////////////////////////////////////////////////////////////////////////
