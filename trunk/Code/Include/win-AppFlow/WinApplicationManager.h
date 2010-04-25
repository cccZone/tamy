#pragma once

#include "core-AppFlow\ApplicationManager.h"
#include "core-AppFlow\IWindowMessagesProcessor.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class CTimer;

///////////////////////////////////////////////////////////////////////////////

class WinApplicationManager : public ApplicationManager,
                              public IWindowMessagesProcessor
{
private:
   std::string m_programName;
   CTimer* m_timer;
   
   unsigned long m_lastFrameRate;

public:
   WinApplicationManager(const std::string& programName);
   ~WinApplicationManager();

   bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
   ProcessingCode onStep();
   
   float getTimeElapsed();
};

///////////////////////////////////////////////////////////////////////////////
