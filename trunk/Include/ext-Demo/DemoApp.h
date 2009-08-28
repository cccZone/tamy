#pragma once

/// @file   ext-Demo\DemoApp.h
/// @brief  a simple framework for creating demos

#include <windows.h>
#include "tamy\Tamy.h"
#include "tamy\SimpleTamyConfigurator.h"
#include "core-AppFlow\ApplicationManager.h"
#include "core-AppFlow\Application.h"
#include "core\TNodesSpatialStorage.h"
#include "core\Octree.h"
#include "core-Renderer\RenderableNode.h"


///////////////////////////////////////////////////////////////////////////////

class Tamy;
class Renderer;
class UserInputController;
class RenderingMechanism;
class SkyBoxStorage;
class Camera;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

class DemoRendererDefinition;
typedef TNodesSpatialStorage<RenderableNode, Octree> DynMeshesScene;
class LightsScene;
class DefaultDemoInputController;

///////////////////////////////////////////////////////////////////////////////

class DemoApp : public Application
{
protected:
   Tamy& m_tamy;
   Renderer* m_renderer;

private:
   UserInputController& m_uiController;
   DemoRendererDefinition* m_rendererDefinition;

   Camera* m_camera;
   DefaultDemoInputController* m_demoController;
   DynMeshesScene* m_dynamicMeshesScene;
   LightsScene* m_lightsScene;

public:
   DemoApp(Tamy& tamy);
   virtual ~DemoApp();

   void initialize();
   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}

protected:
   virtual RenderingMechanism* initRenderingPipeline(DemoRendererDefinition& rendererDefinition,
                                                     DynMeshesScene* dynamicScene, 
                                                     LightsScene* lights);
   virtual void initializeScene(DynMeshesScene& dynamicScene, LightsScene& lights) {}
   virtual void onDeinitialize() {}

   /**
    * Override this method if you want to use your own demo input controller.
    *
    * @param uiController  user input system that can be used to check
    *                      for the user's input
    */
   virtual void initInput(UserInputController& uiController);

   /**
    * The method sets a new background on the default
    * DemoRendererDefinition.
    * 
    * @param skyBox  storage with a skbox we want to have
    *                accessible during pipeline assembly under 
    *                <<background>> source
    */
   void setBackground(SkyBoxStorage* skyBox);

   /** 
    * If you implemented 'initRenderingPipeline' method in your demo,
    * but haven't implemented the 'initInput' method, you're probably gonna 
    * need to call this one. It sets up a camera instance
    * of which is used by 'initInput' method to set up a default
    * demo input controller.
    *
    * @param camera     camera to be used as a default demo camera.
    */
   void initDefaultCamera(Camera* camera);

   /** 
    * Call me if you're sick and tired of the demo.
    */
   void endDemo();
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
