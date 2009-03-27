#pragma once

#include "core-AppFlow\ApplicationManager.h"
#include "core\WindowMessagesProcessor.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class D3DResourceManager;
class Renderer;
class CTimer;

///////////////////////////////////////////////////////////////////////////////

class D3DApplicationManager : public ApplicationManager,
                              public IWindowMessagesProcessor
{
private:
   HINSTANCE m_hInstance;
   std::string m_programName;
   HWND m_hWnd;
   D3DResourceManager* m_resourceManager;
   Renderer* m_renderer;
   CTimer* m_timer;

   unsigned long m_lastFrameRate;

   bool m_rightMouseButton;
   bool m_leftMouseButton;

   bool m_relativeMouseMovement;
   POINT m_oldCursorPos;

public:
   D3DApplicationManager(HINSTANCE hInstance, 
                         int nCmdShow, 
                         const std::string& programName);
   ~D3DApplicationManager();

   LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
   ProcessingCode onStep();

   float getTimeElapsed();

   void checkUserInput(unsigned char* keysBuffer, Point& mousePos);

   Renderer& getRenderer();

   ResourceManager& getResourceManager();

   void switchMouseMovementMode(bool relative);

private:
   void advanceApplicationState();
};

///////////////////////////////////////////////////////////////////////////////
