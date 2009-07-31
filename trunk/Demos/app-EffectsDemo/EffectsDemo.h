#pragma once

#include "core-AppFlow\Application.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\GraphicalEffect.h"
#include "core\ResourceStorage.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;
class Skeleton;
class Renderer;
class Tamy;
class RenderingTarget;
class CyclicRenderingTarget;
class PostProcessMechanism;
class SceneRenderingMechanism;
class SettableRenderingTargetsPolicy;
class UserInputController;

///////////////////////////////////////////////////////////////////////////////

class RenderingUpdator
{
private:
   Skeleton& m_animationController;
   Renderer& m_renderer;

public:
   RenderingUpdator(Skeleton& animationController,
                    Renderer& renderer)
         : m_animationController(animationController),
         m_renderer(renderer)
   {}

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

class EffectsDemo : public Application
{
private:
   Tamy& m_tamy;
   Renderer* m_renderer;
   SceneRenderingMechanism* m_sceneRenderer;
   SettableRenderingTargetsPolicy* m_sceneRenderingTargetPolicy;
   CyclicRenderingTarget* m_mainRendererOutput;
   RenderingTarget* m_screenBuffer;

   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;
   Skeleton* m_animationController;

   bool m_rotating;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;
   ResourceStorage<GraphicalEffect> m_effectsStorage;

   RenderingUpdator* m_renderingUpdator;

   UserInputController* m_uiController;

   std::vector<PostProcessMechanism*> m_postProcessEffects;
   PostProcessMechanism* m_currEffect;
   unsigned int m_nextEffectIdx;
   float m_timeSinceLastSwitch;

public:
   EffectsDemo(Tamy& tamy);

   void initialize();
   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}

private:
   void switchEffect();
};

///////////////////////////////////////////////////////////////////////////////
