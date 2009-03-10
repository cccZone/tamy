#pragma once

#include "WindowMessagesProcessor.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DResourceManager;
class Renderer;
class CTimer;
class SceneManager;
class UnconstrainedMotionController;
class Skeleton;

///////////////////////////////////////////////////////////////////////////////

class AnimationDemo : public IWindowMessagesProcessor
{
private:
   HINSTANCE m_hInstance;
   HWND m_hWnd;
   D3DResourceManager* m_resourceManager;
   Renderer* m_renderer;
   CTimer* m_timer;

   POINT m_oldCursorPos;
   unsigned long m_lastFrameRate;

   SceneManager* m_sceneManager;

   UnconstrainedMotionController* m_cameraController;
   Skeleton* m_animationController;

public:
   AnimationDemo(HINSTANCE hInstance);
   ~AnimationDemo();

   void run(int nCmdShow);
   
   LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
   void advanceGameState();
   void processInput(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////
