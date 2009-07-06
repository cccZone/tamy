#pragma once

#include "core-AppFlow\ApplicationManager.h"
#include "core\Point.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class ApplicationManagerMock : public ApplicationManager
{
private:
   int m_keysDown;
   Point m_mousePos;
   unsigned char m_keyBuffer[256];

public:
   ApplicationManagerMock()
   {
      ZeroMemory(m_keyBuffer, 256 * sizeof(unsigned char));
   }


   void keyDown(unsigned char keys)
   {
      m_keyBuffer[keys] = 0xF0;
   }
   
   void mouseMove(const Point& abs)
   {
      m_mousePos = abs;
   }

protected:
   ProcessingCode onStep() {return APC_APPLICATION;}

   float getTimeElapsed() {return 1;}

   void checkUserInput(unsigned char* keyBuffer, Point& mousePos)
   {
      memcpy(keyBuffer, m_keyBuffer, 256 * sizeof(char));
      ZeroMemory(m_keyBuffer, 256 * sizeof(unsigned char));
      mousePos = m_mousePos;
   }

   void switchMouseMovementMode(bool relative)
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
