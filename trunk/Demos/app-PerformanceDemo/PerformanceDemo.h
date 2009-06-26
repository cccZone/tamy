#pragma once

#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class VisualSceneManager;
class UnconstrainedMotionController;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

class PerformanceDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resourceManager;

   CompositeSceneManager* m_sceneManager;
   VisualSceneManager* m_visualSceneManager;
   UnconstrainedMotionController* m_cameraController;

   bool m_rotating;

public:
   PerformanceDemo();

   void initialize(ResourceManager& resourceManager);
   void deinitialize();

   void hibernate(ResourceManager& resourceManager) {}
   void dehibernate(ResourceManager& resourceManager) {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
