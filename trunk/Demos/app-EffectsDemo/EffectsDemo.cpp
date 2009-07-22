#include "EffectsDemo.h"
#include "impl-DirectX\D3DApplicationManager.h"
#include "impl-DirectX\Tamy.h"
#include "core-AppFlow\ExecutionContext.h"
#include "core-Renderer\Renderer.h"
#include "core\Point.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core\CompositeSceneManager.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "core-Renderer\GraphicalEntity.h"
#include "core-Renderer\Skeleton.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\TextureRenderingTarget.h"
#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\SceneRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "core-Renderer\PostProcessMechanism.h"
#include "core-Renderer\GraphicalEffect.h"
#include "core-Renderer\CyclicRenderingTarget.h"
#include "core-Renderer\ChangableTexture.h"
#include "core\dostream.h"


// effect data sources
#include "OldTVDataSource.h"
#include "WavyImageDataSource.h"
#include "DizzyDataSource.h"
#include "BlurEffectDataSource.h"


///////////////////////////////////////////////////////////////////////////////

void RenderingUpdator::update(float timeElapsed)
{
   m_animationController.update(timeElapsed);
   m_renderer.render();
}

///////////////////////////////////////////////////////////////////////////////

EffectsDemo::EffectsDemo(Tamy& tamy)
      : Application("Demo"),
      m_tamy(tamy),
      m_renderer(&(tamy.renderer())),
      m_mainRendererOutput(NULL),
      m_screenBuffer(NULL),
      m_sceneManager(NULL),
      m_cameraController(NULL),
      m_animationController(NULL),
      m_renderingUpdator(NULL),
      m_currEffect(NULL),
      m_nextEffectIdx(0),
      m_timeSinceLastSwitch(0)
{
   timeController().add("regularTrack");
   timeController().get("regularTrack").add(new TTimeDependent<EffectsDemo>(*this));

   timeController().add("renderingTrack");
   timeController().get("renderingTrack").setLimit(60);
}

///////////////////////////////////////////////////////////////////////////////

void EffectsDemo::initialize()
{
   m_rotating = false;
   m_sceneManager = new CompositeSceneManager();
   VisualSceneManager* visualSceneManager = new VisualSceneManager();
   m_sceneManager->addSceneManager(visualSceneManager);

   SceneRenderingMechanism& sceneRenderer = m_tamy.sceneRenderingMechanism();
   sceneRenderer.addVisualSceneManager(*visualSceneManager);

   IWFLoader loader(m_tamy.graphicalFactory(), 
                    m_tamy.meshLoaders(),
                    *m_sceneManager, 
                    m_entitiesStorage,
                    m_renderingTechniquesStorage);
   loader.load("..\\Data\\AnimLandscape.iwf");

   AbstractGraphicalEntity& ent = m_entitiesStorage.get("animlandscape.x");
   m_animationController = ent.instantiateSkeleton(m_sceneManager->root());
   m_animationController->activateAnimation("Cutscene_01", true);

   Light* light = m_tamy.graphicalFactory().createLight("light");
   light->setType(Light::LT_DIRECTIONAL);
   light->setDiffuseColor(Color(1, 1, 1, 0));
   light->setLookVec(D3DXVECTOR3(0, 0, -1));
   m_sceneManager->addNode(light);

   Camera* camera = m_tamy.graphicalFactory().createCamera("camera");
   m_sceneManager->addNode(camera);
   m_cameraController = new UnconstrainedMotionController(*camera);

   // set up the effect rendering sequence
   m_mainRendererOutput = new CyclicRenderingTarget("cyclicRenderingTarget");
   m_mainRendererOutput->add(m_tamy.graphicalFactory().createTextureRenderingTarget("outputTex0"));
   m_mainRendererOutput->add(m_tamy.graphicalFactory().createTextureRenderingTarget("outputTex1"));
   m_mainRendererOutput->add(m_tamy.graphicalFactory().createTextureRenderingTarget("outputTex2"));
   m_mainRendererOutput->add(m_tamy.graphicalFactory().createTextureRenderingTarget("outputTex3"));
   m_mainRendererOutput->add(m_tamy.graphicalFactory().createTextureRenderingTarget("outputTex4"));
   m_screenBuffer = m_tamy.graphicalFactory().createDefaultRenderingTarget();
   m_tamy.sceneRenderingTargetPolicy().addTarget(0, *m_mainRendererOutput);

   PostProcessEffectNode* postProcessEffect = NULL;
   SettableRenderingTargetsPolicy* policy = NULL;
   ChangableTexture* inputTexture = NULL;

   // create the effects:
   // 1. old tv set
   OldTVDataSource* oldTVData = new OldTVDataSource(*m_mainRendererOutput);
   GraphicalEffect* oldTV = m_tamy.graphicalFactory().createEffect("..\\Data\\oldTV.fx", false, oldTVData);
   m_renderingTechniquesStorage.add(oldTV);

   postProcessEffect = m_tamy.graphicalFactory().createPostProcessEffectNode("oldTV", *oldTV);
   policy = new SettableRenderingTargetsPolicy();
   policy->addTarget(0, *m_screenBuffer);
   m_postProcessEffects.push_back(new PostProcessMechanism(policy, postProcessEffect));

   // 2. wavy image
   WavyImageDataSource* wavyImageData = new WavyImageDataSource(*m_mainRendererOutput);
   GraphicalEffect* wavyImage = m_tamy.graphicalFactory().createEffect("..\\Data\\wavyImage.fx", false, wavyImageData);
   m_renderingTechniquesStorage.add(wavyImage);

   postProcessEffect = m_tamy.graphicalFactory().createPostProcessEffectNode("wavyImage", *wavyImage);
   policy = new SettableRenderingTargetsPolicy();
   policy->addTarget(0, *m_screenBuffer);
   m_postProcessEffects.push_back(new PostProcessMechanism(policy, postProcessEffect));

   // 3. dizzy
   DizzyDataSource* dizzyData = new DizzyDataSource(*m_mainRendererOutput);
   GraphicalEffect* dizzy = m_tamy.graphicalFactory().createEffect("..\\Data\\dizzy.fx", false, dizzyData);
   m_renderingTechniquesStorage.add(dizzy);

   postProcessEffect = m_tamy.graphicalFactory().createPostProcessEffectNode("dizzy", *dizzy);
   policy = new SettableRenderingTargetsPolicy();
   policy->addTarget(0, *m_screenBuffer);
   m_postProcessEffects.push_back(new PostProcessMechanism(policy, postProcessEffect));

   // 4. blur
   BlurEffectDataSource* blurData = new BlurEffectDataSource(*m_mainRendererOutput);
   GraphicalEffect* blur = m_tamy.graphicalFactory().createEffect("..\\Data\\blur.fx", false, blurData);
   m_renderingTechniquesStorage.add(blur);

   postProcessEffect = m_tamy.graphicalFactory().createPostProcessEffectNode("blur", *blur);
   policy = new SettableRenderingTargetsPolicy();
   policy->addTarget(0, *m_screenBuffer);
   m_postProcessEffects.push_back(new PostProcessMechanism(policy, postProcessEffect));

   m_renderingUpdator = new RenderingUpdator(*m_animationController, *m_renderer);
   timeController().get("renderingTrack").add(new TTimeDependent<RenderingUpdator>(*m_renderingUpdator));

   // set the first effect
   switchEffect();
}

///////////////////////////////////////////////////////////////////////////////

void EffectsDemo::deinitialize()
{
   delete m_renderingUpdator;
   m_renderingUpdator = NULL;
   
   delete m_screenBuffer;
   m_screenBuffer = NULL;

   delete m_mainRendererOutput;
   m_mainRendererOutput = NULL;

   delete m_animationController;
   m_animationController = NULL;

   delete m_cameraController;
   m_cameraController = NULL;

   delete m_sceneManager;
   m_sceneManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EffectsDemo::update(float timeElapsed)
{
   float movementSpeed = 40 * timeElapsed;
   float rotationSpeed = 180 * timeElapsed;


   // process the keys
   if (context().isKeyPressed(VK_ESCAPE))    {context().signal(*this, ON_EXIT);}
   if (context().isKeyPressed(VK_UP))     {m_cameraController->move(m_cameraController->getLookVec()   * movementSpeed);}
   if (context().isKeyPressed(VK_DOWN))   {m_cameraController->move(-m_cameraController->getLookVec()  * movementSpeed);}
   if (context().isKeyPressed(VK_LEFT))   {m_cameraController->move(-m_cameraController->getRightVec() * movementSpeed);}
   if (context().isKeyPressed(VK_RIGHT))  {m_cameraController->move(m_cameraController->getRightVec()  * movementSpeed);}

   if (context().isKeyPressed(VK_LBUTTON) && (m_rotating == false))
   {
      context().relativeMouseMovement(true);
      m_rotating = true;
   }
   else if ((context().isKeyPressed(VK_LBUTTON) == false) && m_rotating)
   {
      context().relativeMouseMovement(false);
      m_rotating = false;
   }

   // process the mouse
   if (m_rotating)
   {
      Point mouseRel = context().getMousePos();
      float rotX = (float)(mouseRel.x) / 3.0f;
      float rotY = (float)(mouseRel.y) / 3.0f;
      m_cameraController->rotate(rotY * rotationSpeed, rotX * rotationSpeed, 0);
   }

   // effects switching
   m_timeSinceLastSwitch += timeElapsed;
   if ((context().isKeyPressed(VK_SPACE)) && (m_timeSinceLastSwitch > 0.4))
   {
      switchEffect();
      m_timeSinceLastSwitch = 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

void EffectsDemo::switchEffect()
{
   m_renderer->removeMechanism(*m_currEffect);

   m_currEffect = m_postProcessEffects.at(m_nextEffectIdx);
   m_renderer->addMechanism(m_currEffect);

   m_nextEffectIdx = (m_nextEffectIdx + 1) % m_postProcessEffects.size();
}

///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR    lpCmdLine,
                   int       nCmdShow)
{
   Tamy tamy("..\\Data", "..\\Data", "..\\Data");
   D3DApplicationManager applicationManager(hInstance, nCmdShow, "Effects Demo", tamy);
	EffectsDemo app(tamy);

   applicationManager.addApplication(app);
   applicationManager.setEntryApplication(app.getName());

   while (applicationManager.step()) {Sleep(0);}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
