#pragma once

#pragma once

#include "core-AppFlow\Application.h"


///////////////////////////////////////////////////////////////////////////////

class SceneManager;
class UnconstrainedMotionController;
class WavFile;
class SoundDevice;
class SoundChannel;
class SoundListener;
class SoundSceneManager;
class SoundRenderer;

///////////////////////////////////////////////////////////////////////////////

class SoundDemo : public Application
{
private:
   Renderer* m_renderer;
   ResourceManager* m_resourceManager;

   SceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;

   bool m_rotating;

   WavFile* m_sound;
   SoundChannel* m_soundChannel;
   SoundListener* m_soundListener;
   SoundSceneManager* m_soundScene;
   SoundRenderer* m_soundRenderer;

public:
   SoundDemo();

   void initialize(Renderer& renderer, ResourceManager& resourceManager);

   void deinitialize();

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
