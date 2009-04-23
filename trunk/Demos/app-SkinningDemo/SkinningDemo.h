#pragma once

#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;
class Skeleton;

///////////////////////////////////////////////////////////////////////////////

class SkinningDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resourceManager;

   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;
   Skeleton* m_animationController;

   bool m_rotating;

public:
   SkinningDemo();

   void initialize(Renderer& renderer, ResourceManager& resourceManager);

   void deinitialize();

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
