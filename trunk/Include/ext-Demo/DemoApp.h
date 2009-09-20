#pragma once

/// @file   ext-Demo\DemoApp.h
/// @brief  a simple framework for creating demos

#include <windows.h>
#include "tamy\Tamy.h"
#include "tamy\SimpleTamyConfigurator.h"
#include "core-AppFlow\ApplicationManager.h"
#include "core-AppFlow\Application.h"
#include "core\TNodesSpatialStorage.h"
#include "core\LinearStorage.h"
#include "core-Renderer\RenderableNode.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Tamy;
class Renderer;
class UserInputController;
class RenderingTarget;
class Camera;
class RenderingMechanism;
class SkyBoxStorage;
class TextField;
class Overlay;
class Font;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

typedef TNodesSpatialStorage<RenderableNode, LinearStorage> DynMeshesScene;
class LightsScene;
class DefaultDemoInputController;
class StaticSceneManager;
class FpsCounter;

///////////////////////////////////////////////////////////////////////////////

class DemoApp : public Application
{
protected:
   Tamy& m_tamy;
   Renderer* m_renderer;
   UserInputController& m_uiController;

private:
   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

   DefaultDemoInputController* m_demoController;

   Font* m_font;
   TextField* m_fpsView;
   FpsCounter* m_fpsCounter;

public:
   DemoApp(Tamy& tamy);
   virtual ~DemoApp();

   virtual void initialize() {};
   virtual void deinitialize() {};

   void hibernate() {}
   void dehibernate() {}

   void notify(const std::string& senderApp, int signalCode) {}

protected:

   /**
    * This method creates a default demo input controller.
    *
    * @param camera  camera the controller should control
    */
   virtual void createDefaultInput(Camera& camera);

   // -------------------------------------------------------------------------
   // Utility methods
   // -------------------------------------------------------------------------

   /** 
    * Call me if you're sick and tired of the demo.
    */
   void endDemo();

   void loadIWF(const std::string& fileName,
                SkyBoxStorage** outSkyBox,
                StaticSceneManager** outStaticScene,
                DynMeshesScene** outDynamicScene, 
                LightsScene** outLights);

   Overlay& getFpsView();

   ResourceStorage<AbstractGraphicalEntity>& getEntitiesStorage();

   ResourceStorage<Material>& getMaterialsStorage();

};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo

///////////////////////////////////////////////////////////////////////////////

#define DEMO(AppClass)                                                        \
int WINAPI WinMain(HINSTANCE hInstance,                                       \
                   HINSTANCE hPrevInstance,                                   \
                   LPSTR    lpCmdLine,                                        \
                   int       nCmdShow)                                        \
{                                                                             \
   SimpleTamyConfigurator configurator(1024, 768, false);                     \
   Tamy::initialize(hInstance, nCmdShow, #AppClass, configurator);            \
                                                                              \
	AppClass app(TAMY);                                                        \
                                                                              \
   ApplicationManager& appMgr = TAMY.appManager();                            \
                                                                              \
   appMgr.addApplication(app);                                                \
   appMgr.setEntryApplication(app.getName());                                 \
                                                                              \
   while (appMgr.step()) {Sleep(0);}                                          \
	return 0;                                                                  \
}

///////////////////////////////////////////////////////////////////////////////
