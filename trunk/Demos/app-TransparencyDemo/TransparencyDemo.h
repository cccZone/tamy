#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;
class Renderer;
class Tamy;
class RenderingTarget;

///////////////////////////////////////////////////////////////////////////////

class TransparencyDemo : public Application
{
private:
   Renderer* m_renderer;
   Tamy& m_tamy;
   RenderingTarget* m_renderingTarget;

   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;
   bool m_rotating;

public:
   TransparencyDemo(Tamy& tamy);

   void initialize();
   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
