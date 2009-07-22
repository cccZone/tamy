#include "impl-DirectX\Tamy.h"

// graphical system headers
#include "impl-DirectX\D3DInitializer.h"
#include "impl-DirectX\D3DRenderer.h"
#include "impl-DirectX\D3DSceneRenderingMechanism.h"
#include "impl-DirectX\D3DGraphicalEntitiesFactory.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"

// sound system headers
#include "impl-DirectX\OpenALSoundSystem.h"
#include "impl-DirectX\SoundInitializer.h"
#include "impl-DirectX\SoundDeviceInfo.h"
#include "impl-DirectX\OALSoundDevice.h"
#include "impl-DirectX\OALSoundListener.h"
#include "impl-DirectX\OALSound3D.h"
#include "core-Sound\SoundRenderer.h"
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

Tamy::Tamy(const std::string& texturesDir,
           const std::string& fontsDir,
           const std::string& meshesDir)
      : m_texturesDir(texturesDir),
      m_fontsDir(fontsDir),
      m_meshesDir(meshesDir),
      m_d3d9(NULL),
      m_d3dInitializer(NULL),
      m_renderer(NULL),
      m_soundDevice(NULL),
      m_soundRenderer(NULL),
      m_sceneRenderingMech(NULL),
      m_sceneRenderingTargetsPolicy(NULL),
      m_graphicalFactory(NULL),
      m_soundFactory(NULL),
      m_meshLoaders(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

void Tamy::initialize(HWND mainAppWindow)
{
   // 1.) graphical system
   m_renderer = createRenderer(mainAppWindow, true);
   m_sceneRenderingTargetsPolicy = new SettableRenderingTargetsPolicy();
   m_sceneRenderingMech = new D3DSceneRenderingMechanism(m_sceneRenderingTargetsPolicy,
                                                         m_renderer->getMaxLightsCount(),
                                                         m_renderer->getD3Device());
   m_renderer->addMechanism(m_sceneRenderingMech);
   m_graphicalFactory = new D3DGraphicalEntitiesFactory(m_texturesDir, 
                                                        m_renderer->getD3Device(), 
                                                        *m_renderer);

   // 2.) sound system
   OpenALSoundSystem* soundSystem = new OpenALSoundSystem();
   SoundInitializer soundInit(*soundSystem);
   SoundDeviceInfo idealDevice(OALV_UNKNOWN, 8);
   const SoundDeviceInfo& bestDevice = soundInit.findBest(idealDevice);
   m_soundDevice = soundSystem->createDevice(bestDevice);
   m_soundRenderer = new SoundRenderer(*m_soundDevice);
   m_soundFactory = new OALSoundEntitiesFactory(*soundSystem);

   // 3.) resources management
   m_meshLoaders = new CompositeGraphicalDataSource(m_meshesDir);
   m_meshLoaders->addSource(new XFileGraphicalEntityLoader(m_renderer->getD3Device()));

}

///////////////////////////////////////////////////////////////////////////////

Tamy::~Tamy()
{
   delete m_meshLoaders;
   m_meshLoaders = NULL;

   delete m_soundFactory;
   m_soundFactory = NULL;

   delete m_graphicalFactory;
   m_graphicalFactory = NULL;

   m_sceneRenderingMech = NULL;
   m_sceneRenderingTargetsPolicy = NULL;

   delete m_renderer;
   m_renderer = NULL;
   
   delete m_soundRenderer;
   m_soundRenderer = NULL;

   delete m_soundDevice;
   m_soundDevice = NULL;

   delete m_d3dInitializer;
   m_d3dInitializer = NULL;

   if (m_d3d9 != NULL)
   {
      m_d3d9->Release();
      m_d3d9 = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

D3DRenderer* Tamy::createRenderer(HWND hWnd, bool windowed)
{
   m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
   if (m_d3d9 == NULL)
   {
      throw std::logic_error(std::string("Cannot initialize DirectX library"));
   }

   m_d3dInitializer = new D3DInitializer(*m_d3d9, hWnd, *this);
   if (m_d3dInitializer == NULL)
   {
      m_d3d9->Release();
      m_d3d9 = NULL;
      throw std::logic_error(std::string("Failed to create a Direct3D initializer"));
   }

   D3DSettings d3DSettings;
   if (windowed)
   {
      d3DSettings = m_d3dInitializer->findBestWindowedMode();
   }
   else
   {
      D3DDISPLAYMODE matchMode;
      matchMode.Width = 1024;
      matchMode.Width = 768;
      matchMode.Format = D3DFMT_X8R8G8B8;
      d3DSettings = m_d3dInitializer->findBestFullscreenMode(matchMode);
   }
   D3DRenderer* renderer = m_d3dInitializer->createDisplay(d3DSettings, hWnd);

   return renderer;
}

///////////////////////////////////////////////////////////////////////////////

bool Tamy::checkDeviceCaps(const D3DCAPS9& caps)
{
   if(!(caps.FVFCaps & D3DFVFCAPS_PSIZE)) {return false;}
   if(caps.MaxPointSize <= 1.0f) {return false;}

   return true;
}

///////////////////////////////////////////////////////////////////////////////

SoundRenderer& Tamy::soundRenderer() 
{
   ASSERT(m_soundRenderer != NULL, "Tamy is not initialized");
   return *m_soundRenderer;
}

///////////////////////////////////////////////////////////////////////////////

SoundDevice& Tamy::soundDevice()
{
   ASSERT(m_soundRenderer != NULL, "Tamy is not initialized");
   return *m_soundDevice;
}

///////////////////////////////////////////////////////////////////////////////

Renderer& Tamy::renderer()
{
   ASSERT(m_renderer != NULL, "Tamy is not initialized");
   return *m_renderer;
}

///////////////////////////////////////////////////////////////////////////////

SceneRenderingMechanism& Tamy::sceneRenderingMechanism()
{
   ASSERT(m_sceneRenderingMech != NULL, "Tamy is not initialized");
   return *m_sceneRenderingMech;
}

///////////////////////////////////////////////////////////////////////////////

SettableRenderingTargetsPolicy& Tamy::sceneRenderingTargetPolicy()
{
   ASSERT(m_sceneRenderingTargetsPolicy != NULL, "Tamy is not initialized");
   return *m_sceneRenderingTargetsPolicy;
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntitiesFactory& Tamy::graphicalFactory()
{
   ASSERT(m_graphicalFactory != NULL, "Tamy is not initialized");
   return *m_graphicalFactory;
}

///////////////////////////////////////////////////////////////////////////////

SoundEntitiesFactory& Tamy::soundFactory()
{
   ASSERT(m_soundFactory != NULL, "Tamy is not initialized");
   return *m_soundFactory;
}

///////////////////////////////////////////////////////////////////////////////

GraphicalDataSource& Tamy::meshLoaders()
{
   ASSERT(m_meshLoaders != NULL, "Tamy is not initialized");
   return *m_meshLoaders;
}

///////////////////////////////////////////////////////////////////////////////
