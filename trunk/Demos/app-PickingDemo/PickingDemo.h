#pragma once

#include "core-AppFlow\Application.h"
#include "core\Array.h"
#include "ext-MotionControllers\WaypointCameraController.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class VisualSceneManager;
class NodeActionsExecutor;
class Node;

///////////////////////////////////////////////////////////////////////////////

class LinearTimeFunc
{
public:
   float operator()(const float& advancement) const
   {
      ASSERT(advancement <= 1, "The value for advancement should be <= 1");
      ASSERT(advancement >= 0, "The value for advancement should be >= 0");

      return sin(advancement * D3DX_PI / 2.f);
   }
};

///////////////////////////////////////////////////////////////////////////////

class PickingDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resourceManager;

   CompositeSceneManager* m_sceneManager;
   VisualSceneManager * m_visualSceneManager;

   NodeActionsExecutor* m_actionsExecutor;
   WaypointCameraController<LinearTimeFunc>* m_cameraController;
   int m_shownNode;

public:
   PickingDemo();

   void initialize(Renderer& renderer, ResourceManager& resourceManager);

   void deinitialize();

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}

private:
   void performQuery(Array<Node*>& nodes);

   void jumpToNext();
};

///////////////////////////////////////////////////////////////////////////////
