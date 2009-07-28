#pragma once

#include "core-AppFlow\Application.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\RenderingTechnique.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class Tamy;
class Renderer;
class CompositeSceneManager;
class UnconstrainedMotionController;
class ResourceManager;
class AbstractGraphicalEntity;
class RenderingTarget;
class UserInputController;

///////////////////////////////////////////////////////////////////////////////

class ComplexSceneDemo : public Application
{
private:
   Tamy& m_tamy;
   Renderer* m_renderer;
   RenderingTarget* m_renderingTarget;
   UserInputController& m_uiController;

   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;
   bool m_rotating;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<RenderingTechnique> m_renderingTechniquesStorage;

public:
   ComplexSceneDemo(Tamy& tamy);

   void initialize();
   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
