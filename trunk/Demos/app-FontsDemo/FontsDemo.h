#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

class FontsDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resourceManager;

   CompositeSceneManager* m_sceneManager;

public:
   FontsDemo();

   void initialize(ResourceManager& resourceManager);
   void deinitialize();

   void hibernate(ResourceManager& resourceManager) {}
   void dehibernate(ResourceManager& resourceManager) {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
