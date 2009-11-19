#include "CameraController.h"
#include "core.h"
#include "core-AppFlow.h"
#include "core-Renderer\Renderer.h"
#include "ext-MotionControllers.h"
#include "CameraCommand.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

CameraController::CameraController(UnconstrainedMotionController& cameraController,
                                   UserInputController& uiController,
                                   Renderer& renderer)
: m_cameraController(cameraController)
, m_uiController(uiController)
, m_renderer(renderer)
, m_rotating(false)
, m_forward(false)
, m_backward(false)
, m_left(false)
, m_right(false)
{
}

///////////////////////////////////////////////////////////////////////////////

CameraController::~CameraController()
{
   for (CommandsMap::iterator it = m_buttonPressedCommands.begin();
        it != m_buttonPressedCommands.end(); ++it)
   {
      delete it->second;
   }
   m_buttonPressedCommands.clear();
}

///////////////////////////////////////////////////////////////////////////////

void CameraController::addButtonPressCommand(unsigned char keyCode, 
                                             CameraCommand* command)
{
   if (command == NULL)
   {
      throw std::invalid_argument("NULL pointer instead CameraCommand instance");
   }

   CommandsMap::iterator it = m_buttonPressedCommands.find(keyCode);
   if (it != m_buttonPressedCommands.end())
   {
      delete it->second;
      it->second = command;
   }
   else
   {
      m_buttonPressedCommands.insert(std::make_pair(keyCode, command));
   }
}

///////////////////////////////////////////////////////////////////////////////

void CameraController::keySmashed(unsigned char keyCode) 
{
   if (m_rotating == true) 
   {
      // if the camera is rotating, we don't want to execute any actions
      return;
   }

   CommandsMap::iterator it = m_buttonPressedCommands.find(keyCode);
   if (it == m_buttonPressedCommands.end())
   {
      return;
   }
   Point mousePos = m_uiController.getMousePos();
   D3DXVECTOR2 viewportPos;
   m_renderer.screenToViewport(mousePos, viewportPos);
   it->second->execute(viewportPos);
}

///////////////////////////////////////////////////////////////////////////////

void CameraController::keyHeld(unsigned char keyCode)
{
   if (keyCode == VK_RBUTTON)
   {
      m_rotating = true;
      m_uiController.setRelativeMouseMovement(true);
   }

   if (keyCode == 'W') {m_forward = true;}
   if (keyCode == 'S') {m_backward = true;}
   if (keyCode == 'A') {m_left = true;}
   if (keyCode == 'D') {m_right = true;}
}

///////////////////////////////////////////////////////////////////////////////

void CameraController::keyReleased(unsigned char keyCode) 
{
   if (keyCode == VK_RBUTTON)
   {
      m_rotating = false;
      m_uiController.setRelativeMouseMovement(false);
   }

   if (keyCode == 'W') {m_forward = false;}
   if (keyCode == 'S') {m_backward = false;}
   if (keyCode == 'A') {m_left = false;}
   if (keyCode == 'D') {m_right = false;}
}

///////////////////////////////////////////////////////////////////////////////

void CameraController::update(float timeElapsed)
{
   float movementSpeed = 40 * timeElapsed;
   float rotationSpeed = 0.1f * timeElapsed;

   // process the keys
   if (m_forward)    {m_cameraController.move(m_cameraController.getLookVec()   * movementSpeed);}
   if (m_backward)   {m_cameraController.move(-m_cameraController.getLookVec()  * movementSpeed);}
   if (m_left)       {m_cameraController.move(-m_cameraController.getRightVec() * movementSpeed);}
   if (m_right)      {m_cameraController.move(m_cameraController.getRightVec()  * movementSpeed);}

   if (m_rotating)
   {
      D3DXVECTOR2 mouseSpeed = m_uiController.getMouseSpeed() * rotationSpeed;
      m_cameraController.rotate(mouseSpeed.y, mouseSpeed.x, 0);
   }
}

///////////////////////////////////////////////////////////////////////////////
