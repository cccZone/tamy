#include "ext-Demo\DefaultDemoInputController.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core-AppFlow\UserInputController.h"
#include "core-Renderer\Camera.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

DefaultDemoInputController::DefaultDemoInputController(UserInputController& uiController,
                                                       Camera& camera,
                                                       EndDemoCommand endDemoCommand)
: m_uiController(uiController)
, m_cameraController(new UnconstrainedMotionController(camera))
, m_endDemo(endDemoCommand)
, m_rotating(false)
{
}

///////////////////////////////////////////////////////////////////////////////

void DefaultDemoInputController::update(float timeElapsed)
{
   float movementSpeed = 40 * timeElapsed;
   float rotationSpeed = 0.1f * timeElapsed;

   // process the keys
   if (m_uiController.isKeyPressed(VK_ESCAPE))     {m_endDemo();}
   if (m_uiController.isKeyPressed('W'))     {m_cameraController->move(m_cameraController->getLookVec()   * movementSpeed);}
   if (m_uiController.isKeyPressed('S'))   {m_cameraController->move(-m_cameraController->getLookVec()  * movementSpeed);}
   if (m_uiController.isKeyPressed('A'))   {m_cameraController->move(-m_cameraController->getRightVec() * movementSpeed);}
   if (m_uiController.isKeyPressed('D'))  {m_cameraController->move(m_cameraController->getRightVec()  * movementSpeed);}

   if (m_uiController.isKeyPressed(VK_LBUTTON) && (m_rotating == false))
   {
      m_uiController.setRelativeMouseMovement(true);
      m_rotating = true;
   }
   else if ((m_uiController.isKeyPressed(VK_LBUTTON) == false) && m_rotating)
   {
      m_uiController.setRelativeMouseMovement(false);
      m_rotating = false;
   }

   // process the mouse
   if (m_rotating)
   {
      D3DXVECTOR2 mouseSpeed = m_uiController.getMouseSpeed() * rotationSpeed;
      m_cameraController->rotate(mouseSpeed.y, mouseSpeed.x, 0);
   }
}

///////////////////////////////////////////////////////////////////////////////
