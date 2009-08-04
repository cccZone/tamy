#pragma once

#include "core\WindowMessagesProcessor.h"
#include <d3d9.h>
#include <vector>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class GraphicalEntitiesFactory;
class UserInputController;
class CompositeGraphicalDataSource;
class SceneRenderingMechanism;
class ApplicationManager;
class SoundDevice;
class SoundRenderer;
class SoundEntitiesFactory;
class GraphicalDataSource;
class RenderingTargetsPolicy;
class UserInputController;
struct RenderingDevice;
class DeviceFilter;
class TamyConfigurator;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is the engine factory class.
 * It will build proper implementaiton dependent versions
 * of the basic engine components
 */
class Tamy
{
private:
   static Tamy s_theInstance;

   HWND m_hWnd;
   CompositeWindowMessagesProcessor* m_winMsgsProcessor;
   IDirect3D9* m_d3d9;
   std::vector<RenderingDevice*> m_devicesDB;

   CompositeGraphicalDataSource* m_meshLoaders;

   SoundRenderer* m_soundRenderer;

public:
   ~Tamy();

   static void initialize(HINSTANCE hInstance,
                          int nCmdShow,
                          const std::string& appName,
                          TamyConfigurator& configurator);

   static Tamy& getInstance() {return s_theInstance;}

   Renderer& renderer();

   GraphicalEntitiesFactory& graphicalFactory();

   const std::vector<RenderingDevice*>& renderingDevicesDB() const {return m_devicesDB;}

   /**
    * The method returns the data source which can be used to load
    * supported types of mesh files
    */
   GraphicalDataSource& meshLoaders();

   ApplicationManager& appManager();

   UserInputController& uiController();

   /**
    * This method will create an instance of the UserInputController clas
    */
   UserInputController* createUserInputController();

   SoundDevice& soundDevice();

   SoundRenderer& soundRenderer();

   SoundEntitiesFactory& soundFactory();

private:
   Tamy();
   Tamy(const Tamy& rhs) {}

   void createRenderingDevicesDB();

   void createMainWindow(HINSTANCE hInstance,
                         int nCmdShow,
                         const std::string& appName,
                         unsigned int winWidth,
                         unsigned int winHeight,
                         bool windowed,
                         HICON icon);

   void createRenderer(HWND window, RenderingDevice& device);
   IDirect3D9* initializeRenderingSystem();
   void addImplSpecificRenderDeviceFilters(IDirect3D9& d3d9, DeviceFilter& filter);
   void createRenderer(IDirect3D9& d3d9, RenderingDevice& deviceDesc, HWND window);
   void destroyRenderer();

   void registerMeshLoaders(CompositeGraphicalDataSource& composite);

   void createAppManager(const std::string& appName);
   void destroyAppManager();

   void createSoundSystem();
   void destroySoundSystem();
};

///////////////////////////////////////////////////////////////////////////////

#define TAMY Tamy::getInstance()

///////////////////////////////////////////////////////////////////////////////
