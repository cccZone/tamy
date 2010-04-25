#include "SceneNavigator.h"
#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "ext-MotionControllers.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include "CameraController.h"
#include "PropertiesEditor.h"


///////////////////////////////////////////////////////////////////////////////

SceneNavigator::SceneNavigator()
: m_cameraController(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

SceneNavigator::~SceneNavigator()
{
   delete m_cameraController; m_cameraController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneNavigator::initialize(TamyEditor& mgr)
{
   Camera& camera = mgr.requestService<Camera>();
   m_cameraController = new UnconstrainedMotionController(camera);

   UserInputController& uiController = mgr.requestService<UserInputController> ();
   Renderer& renderer = mgr.requestService<Renderer> ();
   CameraController* cameraAnimator = new CameraController(*m_cameraController,
      uiController, renderer);

   TimeController& timeController = mgr.requestService<TimeController> ();
   KeysStatusManager& keysMgr = mgr.requestService<KeysStatusManager> ();
   keysMgr.addHandler(cameraAnimator);
   timeController.add("sceneNavigation");
   timeController.get("sceneNavigation").add(new TTimeDependent<CameraController> (*cameraAnimator));

   // register new services
   mgr.registerService<CameraController> (*cameraAnimator);
}

///////////////////////////////////////////////////////////////////////////////
