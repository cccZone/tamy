#include "tamy\Tamy.h"
#include "tamy\TamyConfigurator.h"
#include "tamy\TamyWindowBuilder.h"
#include "core\Filesystem.h"
#include <stdexcept>
#include <windows.h>
#include "core-Sound\SoundRenderer.h"
#include "core-ResourceManagement\CompositeGraphicalDataSource.h"
#include "core-Renderer\DeviceFilter.h"
#include "core-Renderer\DeviceFilterElem.h"
#include "core-Renderer\RenderDevicesEnumerator.h"
#include "core-Renderer\RenderingDevice.h"


///////////////////////////////////////////////////////////////////////////////

Tamy Tamy::s_theInstance;

///////////////////////////////////////////////////////////////////////////////

Tamy::Tamy()
: m_hWnd(NULL)
, m_winMsgsProcessor(NULL)
, m_d3d9(NULL)
, m_filesystem(NULL)
, m_meshLoaders(NULL)
, m_soundRenderer(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

Tamy::~Tamy()
{
   delete m_soundRenderer;
   m_soundRenderer = NULL;

   destroySoundSystem();

   delete m_filesystem;
   m_filesystem = NULL;

   delete m_meshLoaders;
   m_meshLoaders = NULL;

   unsigned int renderDevicesCount = m_devicesDB.size();
   for (unsigned int i = 0; i < renderDevicesCount; ++i)
   {
      delete m_devicesDB[i];
   }
   m_devicesDB.clear();

   destroyAppManager();
   destroyRenderer();

   if (m_d3d9 != NULL)
   {
      m_d3d9->Release();
      m_d3d9 = NULL;
   }

   SetWindowLong(m_hWnd, GWL_USERDATA, (LONG)NULL);
   delete m_winMsgsProcessor;
   m_winMsgsProcessor = NULL;

   DestroyWindow(m_hWnd);
   m_hWnd = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Tamy::initialize(const std::string& appName,
                      TamyConfigurator& configurator,
                      TamyWindowBuilder& windowBuilder)
{
   if (s_theInstance.m_hWnd != NULL)
   {
      throw std::logic_error("Tamy has already been initialized");
   }

   s_theInstance.m_filesystem = new Filesystem();
   s_theInstance.m_filesystem->changeRootDir("..\\Data");

   s_theInstance.m_d3d9 = s_theInstance.initializeRenderingSystem();

   // create the database of all valid rendering devices and their modes,
   // so that the user can decide about the resolution of the window etc.
   s_theInstance.createRenderingDevicesDB();

   RenderingDevice& device = configurator.selectRenderingDevice(s_theInstance.m_devicesDB);

   s_theInstance.m_winMsgsProcessor = new CompositeWindowMessagesProcessor();
   s_theInstance.m_hWnd = windowBuilder.create(*(s_theInstance.m_winMsgsProcessor));

   s_theInstance.createRenderer(*(s_theInstance.m_d3d9), device, s_theInstance.m_hWnd);

   s_theInstance.m_meshLoaders = new CompositeGraphicalDataSource();
   s_theInstance.registerMeshLoaders(*(s_theInstance.m_meshLoaders));

   s_theInstance.createAppManager(appName);

   s_theInstance.createSoundSystem();
   s_theInstance.m_soundRenderer = new SoundRenderer(s_theInstance.soundDevice());
}

///////////////////////////////////////////////////////////////////////////////

void Tamy::createRenderingDevicesDB()
{
   std::vector<RenderingDevice*> allDevicesDB;
   RenderDevicesEnumerator enumerator(*m_d3d9);
   enumerator.getDevices(allDevicesDB);

   // filter out the devices our engine intrinsically can't use due to
   // the way certain operations are implemented
   DeviceFilter filter;
   addImplSpecificRenderDeviceFilters(*m_d3d9, filter);
   std::vector<RenderingDevice*> applicableDevicesDB;
   filter(allDevicesDB, applicableDevicesDB);

   if (applicableDevicesDB.size() == 0)
   {
      throw std::runtime_error("No applicable graphics devices found");
   }

   // let's create the final db of the devices and release 
   // those we no longer need
   unsigned int applicableDevicesCount = applicableDevicesDB.size();
   m_devicesDB.resize(applicableDevicesCount);
   for (unsigned int i = 0; i < applicableDevicesCount; ++i)
   {
      m_devicesDB[i] = new RenderingDevice(*(applicableDevicesDB[i]));
   }

   unsigned int allDevicesCount = allDevicesDB.size();
   for (unsigned int i = 0; i < allDevicesCount; ++i)
   {
      delete allDevicesDB[i];
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

Filesystem& Tamy::filesystem()
{
   ASSERT(m_filesystem != NULL, "Filesystem not initialized by Tamy");
   return *m_filesystem;
}

///////////////////////////////////////////////////////////////////////////////
