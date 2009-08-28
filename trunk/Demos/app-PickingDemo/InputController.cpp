#include "InputController.h"
#include "core\Point.h"
#include "core\Ray.h"
#include "core\Node.h"
#include "core\NodeActionsExecutor.h"
#include "core\Triangle.h"
#include "core-AppFlow\UserInputController.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\ParticleSystem.h"
#include "core-Renderer\Camera.h"


///////////////////////////////////////////////////////////////////////////////

InputController::InputController(UserInputController& uiController,
                                 Renderer& renderer,
                                 NodeActionsExecutor& actionsExecutor,
                                 ParticleSystem& burst,
                                 Node& cursor,
                                 Camera& sceneCamera,
                                 demo::DynMeshesScene& mainScene)
: m_uiController(uiController)
, m_renderer(renderer)
, m_actionsExecutor(actionsExecutor)
, m_burst(burst)
, m_cursor(cursor)
, m_sceneCamera(sceneCamera)
, m_mainScene(mainScene)
{
}

///////////////////////////////////////////////////////////////////////////////

void InputController::update(float timeElapsed)
{
   m_uiController.update(timeElapsed);
   static bool keyPressed = false;
   static bool onPress = false;

   if (m_uiController.isKeyPressed(VK_LBUTTON))
   {
      onPress = (keyPressed == false);
      keyPressed = true;
   }
   else if (m_uiController.isKeyPressed(VK_LBUTTON) == false)
   {
      keyPressed = false;
   }

   if (onPress)
   {
      Array<Node*> nodes;
      performQuery(nodes);

      m_actionsExecutor.execute(nodes);
      m_burst.activate();
   }

   const Point& mouseScreenPos = m_uiController.getMousePos();
   D3DXVECTOR2 mouseViewportPos(0, 0);
   m_renderer.screenToViewport(mouseScreenPos, mouseViewportPos);
   D3DXMatrixTranslation(&(m_cursor.accessLocalMtx()), mouseViewportPos.x, mouseViewportPos.y, 10);
}

///////////////////////////////////////////////////////////////////////////////

void InputController::performQuery(Array<Node*>& nodes)
{
   // construct the query ray
   Point mousePos = m_uiController.getMousePos();

   D3DXVECTOR2 viewportPos;
   m_renderer.screenToViewport(mousePos, viewportPos);

   Ray queryRay = m_sceneCamera.createRay(viewportPos.x, viewportPos.y);

   // perform the query
   Array<RenderableNode*> queriedNodes;
   m_mainScene.detailedQuery(queryRay, queriedNodes);

   // output the results
   for (unsigned int i = 0; i < queriedNodes.size(); ++i)
   {
      nodes.push_back(queriedNodes[i]);
   }
}

///////////////////////////////////////////////////////////////////////////////
