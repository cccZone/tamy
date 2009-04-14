#pragma once

#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class SceneManager;
class UnconstrainedMotionController;
class WavFile;
class SoundDevice;
class SoundChannel;

///////////////////////////////////////////////////////////////////////////////

class SoundDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resourceManager;
   SoundDevice* m_soundDevice;

   SceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;

   bool m_rotating;

   WavFile* m_sound;
   SoundChannel* m_soundChannel;

public:
   SoundDemo();

   void initialize(Renderer& renderer, ResourceManager& resourceManager);

   void deinitialize();

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
