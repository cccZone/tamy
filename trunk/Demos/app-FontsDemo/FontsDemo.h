#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;
class Renderer;
class Tamy;

///////////////////////////////////////////////////////////////////////////////

class FontsDemo : public Application
{
private:
   Renderer* m_renderer;
   Tamy& m_tamy;

   CompositeSceneManager* m_sceneManager;

public:
   FontsDemo(Tamy& tamy);

   void initialize();
   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
