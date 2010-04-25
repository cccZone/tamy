#pragma once

#include "ext-Demo\DemoApp.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class WavFile;
class SoundDevice;
class SoundChannel;
class SoundListener;
class SoundSceneManager;

class Camera;

namespace demo
{
   class BasicRenderingPipeline;
}

///////////////////////////////////////////////////////////////////////////////

class SoundDemo : public demo::DemoApp
{
private:
   Camera* m_camera;
   demo::BasicRenderingPipeline* m_renderingPipeline;

   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

   WavFile* m_sound;
   SoundChannel* m_soundChannel;
   SoundListener* m_soundListener;
   SoundSceneManager* m_audioSoundScene;

public:
   SoundDemo(Tamy& tamy);

   void initialize();
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////
