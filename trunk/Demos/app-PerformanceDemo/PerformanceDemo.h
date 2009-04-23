#pragma once

#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;

///////////////////////////////////////////////////////////////////////////////

class PerformanceDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resourceManager;

   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;

   bool m_rotating;

public:
   PerformanceDemo();

   void initialize(Renderer& renderer, ResourceManager& resourceManager);

   void deinitialize();

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
