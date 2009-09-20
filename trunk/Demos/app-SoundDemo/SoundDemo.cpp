#include "SoundDemo.h"
#include "tamy\Tamy.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\SortingRenderablesStorage.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "ext-Demo\BasicRenderingPipeline.h"
#include "ext-Demo\LightsScene.h"
#include "ext-Demo\SharedOverlay.h"
#include "ext-Demo\RERCreator.h"
#include "core-Sound\SoundDevice.h"
#include "core-Sound\SoundChannel.h"
#include "core-Sound\WavFile.h"
#include "core-Sound\SoundListener.h"
#include "core-Sound\SoundRenderer.h"
#include "core-Sound\SoundSceneManager.h"
#include "core-Sound\Sound3D.h"
#include "core-Sound\SoundEntitiesFactory.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

SoundDemo::SoundDemo(Tamy& tamy)
: DemoApp(tamy)
, m_camera(NULL)
, m_renderingPipeline(NULL)
{
   m_camera = m_tamy.graphicalFactory().createCamera("camera");
   m_renderingPipeline = new BasicRenderingPipeline(m_tamy.graphicalFactory(), m_tamy.renderer(), *m_camera);

   timeController().add("rendererTrack");
   timeController().get("rendererTrack").add(new TTimeDependent<BasicRenderingPipeline> (*m_renderingPipeline));

   timeController().add("soundTrack");
   timeController().get("soundTrack").add(new TTimeDependent<SoundRenderer>(m_tamy.soundRenderer()));
}

///////////////////////////////////////////////////////////////////////////////

void SoundDemo::initialize()
{   
   m_audioSoundScene = new SoundSceneManager();
   m_tamy.soundRenderer().addSoundScene(*m_audioSoundScene);

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 1));
   light->setSpecularColor(Color(0.2f, 0.2f, 0.2f, 1));
   D3DXMatrixRotationYawPitchRoll(&(light->accessLocalMtx()), D3DXToRadian(-45), D3DXToRadian(45), 0);
   demo::LightsScene* lights = new demo::LightsScene();
   lights->insert(light);

   m_soundListener = m_tamy.soundFactory().createListener();
   m_audioSoundScene->setListener(m_soundListener);

   // prepare tiles that emit sounds
   GraphicalEntityLoader loader(m_tamy.graphicalFactory(), m_materialsStorage);
   AbstractGraphicalEntity* ent = loader.load("meadowNormalTile.x", m_tamy.meshLoaders());
   m_entitiesStorage.add(ent);
   GraphicalEntityInstantiator* entInstance = new GraphicalEntityInstantiator("tile");
   entInstance->attachEntity(*ent);

   m_sound = new WavFile("..\\Data\\Footsteps.wav");
   Sound3D* tileSound = m_tamy.soundFactory().createEmiter("tileSound", *m_sound, 100);
   entInstance->addChild(tileSound);
   m_audioSoundScene->addEmitter(*tileSound);

   demo::DynMeshesScene* dynamicScene = new demo::DynMeshesScene(new LinearStorage<RenderableNode>());
   dynamicScene->addNode(entInstance);

   // assemble rendering pipeline
   m_renderingPipeline->setDynamicScene(dynamicScene);
   m_renderingPipeline->setLights(lights);
   m_renderingPipeline->setOverlay(new demo::SharedOverlay(getFpsView()));
   m_renderingPipeline->create();

   // initialize input controller
   createDefaultInput(*m_camera);
}

///////////////////////////////////////////////////////////////////////////////

void SoundDemo::deinitialize()
{   
   timeController().remove("rendererTrack");

   delete m_renderingPipeline;
   m_renderingPipeline = NULL;

   delete m_camera;
   m_camera = NULL;

   m_tamy.soundRenderer().removeSoundScene(*m_audioSoundScene);
   
   delete m_audioSoundScene;
   m_audioSoundScene = NULL;

   delete m_sound;
   m_sound = NULL;
}


///////////////////////////////////////////////////////////////////////////////

DEMO(SoundDemo)

///////////////////////////////////////////////////////////////////////////////
