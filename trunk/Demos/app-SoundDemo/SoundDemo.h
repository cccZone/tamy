#pragma once

#include "core-AppFlow\Application.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\RenderingTechnique.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class CompositeSceneManager;
class UnconstrainedMotionController;
class WavFile;
class SoundDevice;
class SoundChannel;
class SoundListener;
class SoundSceneManager;
class Renderer;
class Tamy;
class RenderingTarget;
class UserInputController;

///////////////////////////////////////////////////////////////////////////////

class SoundDemo : public Application
{
private:
   Renderer* m_renderer;
   Tamy& m_tamy;
   RenderingTarget* m_renderingTarget;
   UserInputController& m_uiController;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<RenderingTechnique> m_renderingTechniquesStorage;

   CompositeSceneManager* m_sceneManager;
   UnconstrainedMotionController* m_cameraController;

   bool m_rotating;

   WavFile* m_sound;
   SoundChannel* m_soundChannel;
   SoundListener* m_soundListener;
   SoundSceneManager* m_audioSoundScene;

public:
   SoundDemo(Tamy& tamy);

   void initialize();
   void deinitialize();

   void hibernate() {}
   void dehibernate() {}

   void update(float timeElapsed);

   void notify(const std::string& senderApp, int signalCode) {}
};

///////////////////////////////////////////////////////////////////////////////
