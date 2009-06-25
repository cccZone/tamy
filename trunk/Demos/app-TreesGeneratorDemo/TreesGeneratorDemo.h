#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;
class Skeleton;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

class TreesGeneratorDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resMgr;

   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;
   Skeleton* m_animationController;

   bool m_rotating;

public:
   TreesGeneratorDemo();

   void initialize(ResourceManager& resMgr);
   void deinitialize();

   void hibernate(ResourceManager& resMgr) {}
   void dehibernate(ResourceManager& resMgr) {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
