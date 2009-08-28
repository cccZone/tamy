#pragma once

#include "ext-Demo\DemoApp.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

class RenderingMechanism;
class NodeActionsExecutor;
class Node;
class ParticleSystem;
class WaypointCameraController;
class Camera;
class InputController;

namespace demo
{
   class DemoRendererDefinition;
};

///////////////////////////////////////////////////////////////////////////////

class PickingDemo : public demo::DemoApp
{
private:
   Camera* m_sceneCamera;
   Camera* m_hudCamera;
   demo::DynMeshesScene* m_hudMeshesScene;
   demo::LightsScene* m_hudLightsScene;
   demo::DynMeshesScene* m_mainScene;

   ParticleSystem* m_atmosphere;
   ParticleSystem* m_cursor;
   ParticleSystem* m_burst;
   ParticleSystem* m_circular;

   NodeActionsExecutor* m_actionsExecutor;
   WaypointCameraController* m_cameraController;
   int m_shownNode;

   InputController* m_inputController;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

public:
   PickingDemo(Tamy& tamy);

protected:
   RenderingMechanism* initRenderingPipeline(demo::DemoRendererDefinition& rendererDefinition,
                                             demo::DynMeshesScene* dynamicScene, 
                                             demo::LightsScene* lights);
   void initializeScene(demo::DynMeshesScene& dynamicScene, demo::LightsScene& lights);
   void onDeinitialize();

   void initInput(UserInputController& uiController);
};

///////////////////////////////////////////////////////////////////////////////
