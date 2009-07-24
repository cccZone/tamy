#pragma once

#include "core-AppFlow\ApplicationManager.h"
#include "core\WindowMessagesProcessor.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class Tamy;
class Renderer;
class CTimer;

///////////////////////////////////////////////////////////////////////////////

class D3DApplicationManager : public ApplicationManager,
                              public IWindowMessagesProcessor
{
private:
   HINSTANCE m_hInstance;
   std::string m_programName;
   Tamy& m_tamy;
   HWND m_hWnd;
   CTimer* m_timer;
   Renderer* m_renderer;
   
   unsigned long m_lastFrameRate;

   bool m_rightMouseButton;
   bool m_leftMouseButton;

   bool m_relativeMouseMovement;
   POINT m_oldCursorPos;

public:
   D3DApplicationManager(HINSTANCE hInstance, 
                         int nCmdShow, 
                         const std::string& programName,
                         Tamy& tamy,
                         HICON largeIcon = NULL,
                         HICON smallIcon = NULL);
   ~D3DApplicationManager();

   HWND getWnd() const {return m_hWnd;}

   LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
   ProcessingCode onStep();
   
   float getTimeElapsed();

   void checkUserInput(unsigned char* keysBuffer, Point& mousePos);

   void switchMouseMovementMode(bool relative);

private:
   void advanceApplicationState();
};

///////////////////////////////////////////////////////////////////////////////
