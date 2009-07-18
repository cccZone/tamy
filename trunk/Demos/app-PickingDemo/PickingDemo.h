#pragma once

#include "core-AppFlow\Application.h"
#include "core\Array.h"
#include <math.h>
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class VisualSceneManager;
class NodeActionsExecutor;
class Node;
class ParticleSystem;
class WaypointCameraController;
class Renderer;
class Tamy;
class RenderingTarget;

///////////////////////////////////////////////////////////////////////////////

class PickingDemo : public Application
{
private:
   Renderer* m_renderer;
   Tamy& m_tamy;
   RenderingTarget* m_renderingTarget;

   CompositeSceneManager* m_sceneManager;
   CompositeSceneManager * m_hudSceneManager;
   VisualSceneManager * m_visualSceneManager;
   
   ParticleSystem* m_atmosphere;
   ParticleSystem* m_cursor;
   ParticleSystem* m_burst;
   ParticleSystem* m_circular;

   NodeActionsExecutor* m_actionsExecutor;
   WaypointCameraController* m_cameraController;
   int m_shownNode;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

public:
   PickingDemo(Tamy& tamy);

   void initialize();

   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}

private:
   void performQuery(Array<Node*>& nodes);
};

///////////////////////////////////////////////////////////////////////////////
