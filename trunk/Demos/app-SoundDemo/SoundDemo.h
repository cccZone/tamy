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

namespace demo
{
   class DemoRendererDefinition;
};

///////////////////////////////////////////////////////////////////////////////

class SoundDemo : public demo::DemoApp
{
private:
   ResourceStorage<AbstractGraphicalEntity> m_entitiesStorage;
   ResourceStorage<Material> m_materialsStorage;

   WavFile* m_sound;
   SoundChannel* m_soundChannel;
   SoundListener* m_soundListener;
   SoundSceneManager* m_audioSoundScene;

public:
   SoundDemo(Tamy& tamy);

protected:
   void initializeScene(demo::DynMeshesScene& dynamicScene, 
                        demo::LightsScene& lights);
   void onDeinitialize();
};

///////////////////////////////////////////////////////////////////////////////
