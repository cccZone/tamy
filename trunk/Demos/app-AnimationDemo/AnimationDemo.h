#pragma once

#include "core-AppFlow\Application.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;
class Skeleton;
class Renderer;
class Tamy;

///////////////////////////////////////////////////////////////////////////////

class AnimationDemo : public Application
{
private:
   Tamy& m_tamy;
   Renderer* m_renderer;

   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;
   Skeleton* m_animationController;

   bool m_rotating;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

public:
   AnimationDemo(Tamy& tamy);

   void initialize();
   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
