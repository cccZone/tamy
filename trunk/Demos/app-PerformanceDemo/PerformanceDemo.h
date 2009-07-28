#pragma once

#include "core-AppFlow\Application.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\RenderingTechnique.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class VisualSceneManager;
class UnconstrainedMotionController;
class Renderer;
class Tamy;
class RenderingTarget;
class UserInputController;

///////////////////////////////////////////////////////////////////////////////

class PerformanceDemo : public Application
{
private:
   Renderer* m_renderer;
   Tamy& m_tamy;
   RenderingTarget* m_renderingTarget;
   UserInputController& m_uiController;

   CompositeSceneManager* m_sceneManager;
   VisualSceneManager* m_visualSceneManager;
   UnconstrainedMotionController* m_cameraController;

   bool m_rotating;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<RenderingTechnique> m_renderingTechniquesStorage;

public:
   PerformanceDemo(Tamy& tamy);

   void initialize();
   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
