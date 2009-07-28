#pragma once

#include "core-AppFlow\ApplicationManager.h"
#include "core\WindowMessagesProcessor.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class CTimer;

///////////////////////////////////////////////////////////////////////////////

class D3DApplicationManager : public ApplicationManager,
                              public IWindowMessagesProcessor
{
private:
   std::string m_programName;
   CTimer* m_timer;
   
   unsigned long m_lastFrameRate;

public:
   D3DApplicationManager(const std::string& programName);
   ~D3DApplicationManager();

   bool wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
   ProcessingCode onStep();
   
   float getTimeElapsed();
};

///////////////////////////////////////////////////////////////////////////////
