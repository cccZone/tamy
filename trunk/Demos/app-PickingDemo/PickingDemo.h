#pragma once

#include "core-AppFlow\Application.h"
#include "core\Array.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class VisualSceneManager;
class NodeActionsExecutor;
class Node;
class ParticleSystem;
class WaypointCameraController;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

class PickingDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resourceManager;

   CompositeSceneManager* m_sceneManager;
   CompositeSceneManager * m_hudSceneManager;
   VisualSceneManager * m_visualSceneManager;
   
   ParticleSystem* m_atmosphere;
   ParticleSystem* m_cursor;

   NodeActionsExecutor* m_actionsExecutor;
   WaypointCameraController* m_cameraController;
   int m_shownNode;

public:
   PickingDemo();

   void initialize(ResourceManager& resourceManager);

   void deinitialize();

   void hibernate(ResourceManager& resourceManager) {}
   void dehibernate(ResourceManager& resourceManager) {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}

private:
   void performQuery(Array<Node*>& nodes);
};

///////////////////////////////////////////////////////////////////////////////
