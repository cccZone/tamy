#include "tamy\Tamy.h"
#include <tchar.h>
#include "core\WindowBuilder.h"
#include <stdexcept>
#include <windows.h>
#include "core-Sound\SoundRenderer.h"
#include "core-ResourceManagement\CompositeGraphicalDataSource.h"


///////////////////////////////////////////////////////////////////////////////

Tamy Tamy::s_theInstance;

///////////////////////////////////////////////////////////////////////////////

Tamy::Tamy()
      : m_hWnd(NULL),
      m_winMsgsProcessor(NULL),
      m_fullScreen(false),
      m_meshLoaders(NULL),
      m_soundRenderer(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

Tamy::~Tamy()
{
   delete m_soundRenderer;
   m_soundRenderer = NULL;

   destroySoundSystem();

   delete m_meshLoaders;
   m_meshLoaders = NULL;

   destroyAppManager();
   destroyRenderer();

   SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)NULL);
   delete m_winMsgsProcessor;
   m_winMsgsProcessor = NULL;

   DestroyWindow(m_hWnd);
   m_hWnd = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Tamy::initialize(HINSTANCE hInstance,
                      int nCmdShow,
                      const std::string& appName,
                      unsigned int winWidth,
                      unsigned int winHeight,
                      bool fullScreen)
{
   if (s_theInstance.m_hWnd != NULL)
   {
      throw std::logic_error("Tamy has already been initialized");
   }

   s_theInstance.createMainWindow(hInstance, 
                                  nCmdShow, 
                                  appName, 
                                  winWidth, 
                                  winHeight, 
                                  fullScreen);

   s_theInstance.createRenderer(s_theInstance.m_hWnd, 
                                winWidth, 
                                winHeight, 
                                fullScreen);

   s_theInstance.m_meshLoaders = new CompositeGraphicalDataSource("..\\Data");  // TODO: zastapic odwolaniem do VFS'a
   s_theInstance.registerMeshLoaders(*(s_theInstance.m_meshLoaders));

   s_theInstance.createAppManager(appName);

   s_theInstance.createSoundSystem();
   s_theInstance.m_soundRenderer = new SoundRenderer(s_theInstance.soundDevice());
}

///////////////////////////////////////////////////////////////////////////////

void Tamy::createMainWindow(HINSTANCE hInstance,
                            int nCmdShow,
                            const std::string& appName,
                            unsigned int winWidth,
                            unsigned int winHeight,
                            bool fullScreen)
{
   m_fullScreen = fullScreen;
   m_winMsgsProcessor = new CompositeWindowMessagesProcessor();

   CWindowBuilder winBuilder;
   WindowParams params;
   strcpy_s(params.windowTitle, appName.c_str());
   strcpy_s(params.windowClassName, (appName + "Class").c_str());
   params.ptrMsgProc = m_winMsgsProcessor;
   params.width = winWidth;
   params.height = winHeight;

   if (fullScreen)
   {
      m_hWnd = winBuilder.createFullScreenWindow(hInstance, nCmdShow, params);
   }
   else
   {
      m_hWnd = winBuilder.createWindowedModeWindow(hInstance, nCmdShow, params);
   }
   if (m_hWnd == NULL)
   {
      std::runtime_error("Application window could not be created");
   }
}

///////////////////////////////////////////////////////////////////////////////

SoundRenderer& Tamy::soundRenderer()
{
   ASSERT(m_soundRenderer != NULL, "SoundRenderer not initialized by Tamy");
   return *m_soundRenderer;
}

///////////////////////////////////////////////////////////////////////////////

GraphicalDataSource& Tamy::meshLoaders()
{
   ASSERT(m_meshLoaders != NULL, "GraphicalDataSource not initialized by Tamy");
   return *m_meshLoaders;
}

///////////////////////////////////////////////////////////////////////////////
