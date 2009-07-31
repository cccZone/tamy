#include "tamy\Tamy.h"

// graphical system headers
#include "impl-DirectX\D3DInitializer.h"
#include "impl-DirectX\D3DRenderer.h"
#include "impl-DirectX\D3DGraphicalEntitiesFactory.h"
#include "impl-DirectX\GraphicalCapsEvaluator.h"

// application management
#include "impl-DirectX\D3DApplicationManager.h"
#include "impl-DirectX\D3DUserInputController.h"

// sound system headers
#include "impl-DirectX\OpenALSoundSystem.h"
#include "impl-DirectX\SoundInitializer.h"
#include "impl-DirectX\SoundDeviceInfo.h"
#include "impl-DirectX\OALSoundDevice.h"
#include "impl-DirectX\OALSoundListener.h"
#include "impl-DirectX\OALSound3D.h"
#include "impl-DirectX\OALSoundEntitiesFactory.h"

// resources management headers
#include "core-ResourceManagement\GraphicalDataSource.h"
#include "impl-DirectX\XFileGraphicalEntityLoader.h"
#include "core-ResourceManagement\CompositeGraphicalDataSource.h"

// other headers
#include <stdexcept>
#include <string>
#include <cassert>
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct TamyGraphImpl : public GraphicalCapsEvaluator
{
   IDirect3D9* m_d3d9;
   D3DRenderer* m_renderer;
   D3DGraphicalEntitiesFactory* m_graphicalFactory;

   // -------------------------------------------------------------------------

   TamyGraphImpl()
      : m_d3d9(NULL),
      m_renderer(NULL),
      m_graphicalFactory(NULL)
   {
   }

   ~TamyGraphImpl()
   {
      delete m_graphicalFactory;
      m_graphicalFactory = NULL;

      delete m_renderer;
      m_renderer = NULL;

      if (m_d3d9 != NULL)
      {
         m_d3d9->Release();
         m_d3d9 = NULL;
      }
   }

   bool checkDeviceCaps(const D3DCAPS9& caps)
   {
      if(!(caps.FVFCaps & D3DFVFCAPS_PSIZE)) {return false;}
      if(caps.MaxPointSize <= 1.0f) {return false;}

      return true;
   }

};

TamyGraphImpl s_tamyGraphImpl; // the singleton instance of the structure

///////////////////////////////////////////////////////////////////////////////

void Tamy::createRenderer(HWND window, 
                          unsigned int winWidth,
                          unsigned int winHeight,
                          bool fullScreen)
{
   ASSERT(s_tamyGraphImpl.m_d3d9 == NULL, "IDirect3D9 instance already exists");
   ASSERT(s_tamyGraphImpl.m_renderer == NULL, "Renderer instance already exists");
   ASSERT(s_tamyGraphImpl.m_graphicalFactory == NULL, "GraphicalEntitiesFactory instance already exists");

   s_tamyGraphImpl.m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
   if (s_tamyGraphImpl.m_d3d9 == NULL)
   {
      throw std::logic_error(std::string("Cannot initialize DirectX library"));
   }

   D3DInitializer d3dInitializer(*(s_tamyGraphImpl.m_d3d9), window, s_tamyGraphImpl);
   D3DSettings d3DSettings;
   if (fullScreen)
   {
      D3DDISPLAYMODE matchMode;
      matchMode.Width = winWidth;
      matchMode.Width = winHeight;
      matchMode.Format = D3DFMT_X8R8G8B8;
      d3DSettings = d3dInitializer.findBestFullscreenMode(matchMode);
   }
   else
   {
      d3DSettings = d3dInitializer.findBestWindowedMode();
   }

   s_tamyGraphImpl.m_renderer = d3dInitializer.createDisplay(d3DSettings, window);
   s_theInstance.m_winMsgsProcessor->add(*(s_tamyGraphImpl.m_renderer));

   s_tamyGraphImpl.m_graphicalFactory = new D3DGraphicalEntitiesFactory("..\\Data",  // TODO: zastapic odwolaniem do VFS'a
                                                                        s_tamyGraphImpl.m_renderer->getD3Device(), 
                                                                        *(s_tamyGraphImpl.m_renderer));
}

///////////////////////////////////////////////////////////////////////////////

void Tamy::destroyRenderer()
{
   delete s_tamyGraphImpl.m_graphicalFactory;
   s_tamyGraphImpl.m_graphicalFactory = NULL;

   delete s_tamyGraphImpl.m_renderer;
   s_tamyGraphImpl.m_renderer = NULL;

   if (s_tamyGraphImpl.m_d3d9 != NULL)
   {
      s_tamyGraphImpl.m_d3d9->Release();
      s_tamyGraphImpl.m_d3d9 = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void Tamy::registerMeshLoaders(CompositeGraphicalDataSource& composite)
{
   ASSERT(s_tamyGraphImpl.m_renderer != NULL, "Renderer instance doesn't exist");
   composite.addSource(new XFileGraphicalEntityLoader(s_tamyGraphImpl.m_renderer->getD3Device()));
}

///////////////////////////////////////////////////////////////////////////////

Renderer& Tamy::renderer()
{
   ASSERT(s_tamyGraphImpl.m_renderer != NULL, "Renderer not initialized by Tamy");
   return *(s_tamyGraphImpl.m_renderer);
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntitiesFactory& Tamy::graphicalFactory()
{
   ASSERT(s_tamyGraphImpl.m_graphicalFactory != NULL, "GraphicalEntitiesFactory not initialized by Tamy");
   return *(s_tamyGraphImpl.m_graphicalFactory);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct TamyAppMgrImpl
{
   D3DApplicationManager* m_appMgr;
   D3DUserInputController* m_uiController;

   // -------------------------------------------------------------------------

   TamyAppMgrImpl()
      : m_appMgr(NULL),
      m_uiController(NULL)
   {
   }

   ~TamyAppMgrImpl()
   {
      delete m_uiController;
      m_uiController = NULL;

      delete m_appMgr;
      m_appMgr = NULL;
   }
};

TamyAppMgrImpl s_tamyAppMgrImpl; // the singleton instance of the structure

///////////////////////////////////////////////////////////////////////////////

void Tamy::createAppManager(const std::string& appName)
{
   ASSERT(s_tamyAppMgrImpl.m_appMgr == NULL, "ApplicationManager instance already exists");
    ASSERT(s_tamyAppMgrImpl.m_uiController == NULL, "UserInputController instance already exists");

   s_tamyAppMgrImpl.m_appMgr = new D3DApplicationManager(appName);
   m_winMsgsProcessor->add(*(s_tamyAppMgrImpl.m_appMgr));

   s_tamyAppMgrImpl.m_uiController = new D3DUserInputController();
   m_winMsgsProcessor->add(*(s_tamyAppMgrImpl.m_uiController));
}

///////////////////////////////////////////////////////////////////////////////

void Tamy::destroyAppManager()
{
   delete s_tamyAppMgrImpl.m_uiController;
   s_tamyAppMgrImpl.m_uiController = NULL;

   delete s_tamyAppMgrImpl.m_appMgr;
   s_tamyAppMgrImpl.m_appMgr = NULL;
}

///////////////////////////////////////////////////////////////////////////////

ApplicationManager& Tamy::appManager()
{
   ASSERT(s_tamyAppMgrImpl.m_appMgr != NULL, "ApplicationManager not initialized by Tamy");
   return *(s_tamyAppMgrImpl.m_appMgr);
}

///////////////////////////////////////////////////////////////////////////////

UserInputController& Tamy::uiController()
{
   ASSERT(s_tamyAppMgrImpl.m_uiController != NULL, "UserInputController not initialized by Tamy");
   return *(s_tamyAppMgrImpl.m_uiController);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct TamySoundImpl
{
   OpenALSoundSystem* m_soundSystem;
   OALSoundDevice* m_soundDevice;
   OALSoundEntitiesFactory* m_soundFactory;

   // -------------------------------------------------------------------------

   TamySoundImpl()
      : m_soundSystem(NULL),
      m_soundDevice(NULL),
      m_soundFactory(NULL)
   {
   }

   ~TamySoundImpl()
   {
      delete m_soundFactory;
      m_soundFactory = NULL;

      delete m_soundDevice;
      m_soundDevice = NULL;

      delete m_soundSystem;
      m_soundSystem = NULL;
   }
};

TamySoundImpl s_tamySoundImpl; // the singleton instance of the structure

///////////////////////////////////////////////////////////////////////////////

void Tamy::createSoundSystem()
{
   ASSERT(s_tamySoundImpl.m_soundSystem == NULL, "OpenALSoundSystem instance already exists");
   ASSERT(s_tamySoundImpl.m_soundDevice == NULL, "SoundDevice instance already exists");
   ASSERT(s_tamySoundImpl.m_soundFactory == NULL, "SoundEntitiesFactory instance already exists");

   s_tamySoundImpl.m_soundSystem = new OpenALSoundSystem();

   SoundInitializer soundInit(*(s_tamySoundImpl.m_soundSystem));
   SoundDeviceInfo idealDevice(OALV_UNKNOWN, 8);
   const SoundDeviceInfo& bestDevice = soundInit.findBest(idealDevice);
   s_tamySoundImpl.m_soundDevice = s_tamySoundImpl.m_soundSystem->createDevice(bestDevice);

   s_tamySoundImpl.m_soundFactory = new OALSoundEntitiesFactory(*(s_tamySoundImpl.m_soundSystem));
}

///////////////////////////////////////////////////////////////////////////////

void Tamy::destroySoundSystem()
{
   delete s_tamySoundImpl.m_soundFactory;
   s_tamySoundImpl.m_soundFactory = NULL;

   delete s_tamySoundImpl.m_soundDevice;
   s_tamySoundImpl.m_soundDevice = NULL;

   delete s_tamySoundImpl.m_soundSystem;
   s_tamySoundImpl.m_soundSystem = NULL;
}

///////////////////////////////////////////////////////////////////////////////

SoundDevice& Tamy::soundDevice()
{
   ASSERT(s_tamySoundImpl.m_soundDevice != NULL, "SoundDevice not initialized by Tamy");
   return *(s_tamySoundImpl.m_soundDevice);
}

///////////////////////////////////////////////////////////////////////////////

SoundEntitiesFactory& Tamy::soundFactory()
{
   ASSERT(s_tamySoundImpl.m_soundFactory != NULL, "SoundEntitiesFactory not initialized by Tamy");
   return *(s_tamySoundImpl.m_soundFactory);
}

///////////////////////////////////////////////////////////////////////////////
