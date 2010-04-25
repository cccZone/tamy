#pragma once

#include "core\Array.h"
#include "ext-Demo\DemoApp.h"


///////////////////////////////////////////////////////////////////////////////

class UserInputController;
class Renderer;
class Node;
class NodeActionsExecutor;
class ParticleSystem;
class Camera;

///////////////////////////////////////////////////////////////////////////////

class InputController
{
private:
   UserInputController& m_uiController;
   Renderer& m_renderer;
   NodeActionsExecutor& m_actionsExecutor;
   ParticleSystem& m_burst;
   Node& m_cursor;
   Camera& m_sceneCamera;
   demo::DynMeshesScene& m_mainScene;

public:
   InputController(UserInputController& uiController, 
                   Renderer& renderer,
                   NodeActionsExecutor& actionsExecutor,
                   ParticleSystem& burst,
                   Node& cursor,
                   Camera& sceneCamera,
                   demo::DynMeshesScene& mainScene);

   void update(float timeElapsed);

private:
   void performQuery(Array<Node*>& nodes);
};

///////////////////////////////////////////////////////////////////////////////
