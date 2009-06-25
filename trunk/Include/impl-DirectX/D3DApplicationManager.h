#pragma once

#include "core-AppFlow\ApplicationManager.h"
#include "core\WindowMessagesProcessor.h"
#include "impl-DirectX\GraphicalCapsEvaluator.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class CTimer;
class D3DInitializer;
class D3DRenderer;

///////////////////////////////////////////////////////////////////////////////

class D3DApplicationManager : public ApplicationManager,
                              public IWindowMessagesProcessor,
                              public GraphicalCapsEvaluator
{
private:
   HINSTANCE m_hInstance;
   std::string m_programName;
   HWND m_hWnd;
   CTimer* m_timer;

   IDirect3D9* m_d3d9;
   D3DInitializer* m_d3dInitializer;
   D3DRenderer* m_renderer;

   unsigned long m_lastFrameRate;

   bool m_rightMouseButton;
   bool m_leftMouseButton;

   bool m_relativeMouseMovement;
   POINT m_oldCursorPos;

public:
   D3DApplicationManager(const std::string& texturesDir,
                         const std::string& fontsDir,
                         const std::string& meshesDir,
                         HINSTANCE hInstance, 
                         int nCmdShow, 
                         const std::string& programName);
   ~D3DApplicationManager();

   LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

   bool checkDeviceCaps(const D3DCAPS9& caps);

protected:
   ProcessingCode onStep();

   float getTimeElapsed();

   void checkUserInput(unsigned char* keysBuffer, Point& mousePos);

   void switchMouseMovementMode(bool relative);

private:
   void advanceApplicationState();

   D3DRenderer* createRenderer(bool windowed);
};

///////////////////////////////////////////////////////////////////////////////
