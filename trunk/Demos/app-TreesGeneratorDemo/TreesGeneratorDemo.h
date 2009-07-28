#pragma once

#include "core-AppFlow\Application.h"
#include "core-Renderer\RenderingTechnique.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;
class Skeleton;
class Renderer;
class ResourceManager;
class AbstractGraphicalEntity;
class Tamy;
class RenderingTarget;
class UserInputController;

///////////////////////////////////////////////////////////////////////////////

class TreesGeneratorDemo : public Application
{
private:
   Renderer* m_renderer;
   Tamy& m_tamy;
   RenderingTarget* m_renderingTarget;
   UserInputController& m_uiController;

   AbstractGraphicalEntity* m_treeEntity;
   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;
   Skeleton* m_animationController;

   bool m_rotating;

   ResourceStorage<RenderingTechnique> m_renderingTechniquesStorage;

public:
   TreesGeneratorDemo(Tamy& tamy);

   void initialize();
   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
