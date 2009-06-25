#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class ResourceManager;
class Renderer;
class CompositeSceneManager;
class UnconstrainedMotionController;

///////////////////////////////////////////////////////////////////////////////

class ComplexSceneDemo : public Application
{
private:
   ResourceManager* m_resMgr;
   Renderer* m_renderer;

   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;
   bool m_rotating;

public:
   ComplexSceneDemo();

   void initialize(ResourceManager& resMgr);
   void deinitialize();

   void hibernate(ResourceManager& resMgr) {}
   void dehibernate(ResourceManager& resMgr) {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
