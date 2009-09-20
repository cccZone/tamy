#include "PostProcessPipeline.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\SortingRenderablesStorage.h"
#include "core-Renderer\LightsStorage.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\CompositeRenderingMechanism.h"
#include "core-Renderer\GeometryRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "core-Renderer\SkyBoxStorage.h"
#include "core-Renderer\RenderingActionsComposite.h"
#include "core-Renderer\RenderMaterial.h"
#include "core-Renderer\RenderLights.h"
#include "core-Renderer\StaticGeometryRenderable.h"
#include "core-Renderer\TargetsCleaningMechanism.h"
#include "core-Renderer\OverlayRenderingMechanism.h"
#include "core-Renderer\Overlay.h"
#include "core-Renderer\TextureRenderingTarget.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\PostProcessSceneStorage.h"
#include "core-Renderer\PostProcessEffectRenderable.h"
#include "ext-Demo\StaticSceneManager.h"
#include "ext-Demo\SharedOverlay.h"
#include "ext-Demo\SharedRenderablesStorage.h"
#include "ext-Demo\RERCreator.h"


///////////////////////////////////////////////////////////////////////////////

PostProcessPipeline::PostProcessPipeline(GraphicalEntitiesFactory& factory,
                                         Renderer& renderer,
                                         Camera& camera)
: m_factory(factory)
, m_renderer(renderer)
, m_camera(camera)
, m_middleTarget(NULL)
, m_screenTarget(NULL)
, m_skyBox(NULL)
, m_staticSceneStorage(NULL)
, m_dynamicSceneStorage(NULL)
, m_staticScene(NULL)
, m_dynamicScene(NULL)
, m_postProcessScene(NULL)
, m_lights(NULL)
, m_overlay(NULL)
, m_rendererImplCreator(NULL)
{
   m_middleTarget = m_factory.createTextureRenderingTarget("middleTarget");
   m_screenTarget = m_factory.createDefaultRenderingTarget();
}

///////////////////////////////////////////////////////////////////////////////

PostProcessPipeline::~PostProcessPipeline()
{
   delete m_middleTarget;
   m_middleTarget = NULL;

   delete m_screenTarget;
   m_screenTarget = NULL;

   delete m_skyBox;
   m_skyBox = NULL;

   delete m_staticScene;
   m_staticScene = NULL;

   delete m_dynamicScene;
   m_dynamicScene = NULL;

   delete m_staticSceneStorage;
   m_staticSceneStorage = NULL;

   delete m_dynamicSceneStorage;
   m_dynamicSceneStorage = NULL;

   delete m_postProcessScene;
   m_postProcessScene = NULL;

   delete m_lights;
   m_lights = NULL;

   delete m_overlay;
   m_overlay = NULL;

   delete m_rendererImplCreator;
   m_rendererImplCreator = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessPipeline::setBackground(RenderablesStorage* bg)
{
   delete m_skyBox;
   m_skyBox = bg;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessPipeline::setStaticScene(demo::StaticSceneManager* stat)
{
   delete m_staticSceneStorage;
   delete m_staticScene;
   
   m_staticSceneStorage = stat;
   if (m_staticSceneStorage != NULL)
   {
      m_staticScene = new SortingRenderablesStorage<StaticGeometryRenderable>(*m_staticSceneStorage);
   }
   else
   {
      m_staticScene = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessPipeline::setDynamicScene(SpatialStorage<RenderableNode>* dyn)
{
   delete m_dynamicSceneStorage;
   delete m_dynamicScene;
   
   m_dynamicSceneStorage = dyn;
   if (m_dynamicSceneStorage != NULL)
   {
      m_dynamicScene = new SortingRenderablesStorage<RenderableNode>(*m_dynamicSceneStorage);
   }
   else
   {
      m_dynamicScene = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessPipeline::setPostProcessScene(PostProcessEffectRenderable* ppRen)
{
   delete m_postProcessScene;
   m_postProcessScene = NULL;

   if (ppRen != NULL)
   {
      ppRen->addInputTexture(*m_middleTarget, m_factory);
      m_postProcessScene = new PostProcessSceneStorage(ppRen);
   }
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessPipeline::setLights(LightsStorage* lights)
{
   delete m_lights;
   m_lights = lights;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessPipeline::setOverlay(Overlay* overlay)
{
   delete m_overlay;
   m_overlay = overlay;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessPipeline::setRendererImpl(const std::string& effectFileName, 
                                          demo::RERCreator* creator)
{
   delete m_rendererImplCreator;
   m_rendererImplCreator = creator;
   m_rendererImplFile = effectFileName;
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessPipeline::update(float timeElapsed)
{
   m_renderer.render();
}

///////////////////////////////////////////////////////////////////////////////

void PostProcessPipeline::create()
{
   CompositeRenderingMechanism* mechanism = new CompositeRenderingMechanism();

   // targets cleaner
   SettableRenderingTargetsPolicy* tcPolicy = new SettableRenderingTargetsPolicy();
   tcPolicy->addTarget(0, *m_middleTarget);
   tcPolicy->addTarget(1, *m_screenTarget);
   RenderingTargetsCleaner* cleaner = m_factory.createTargetsCleaner();
   TargetsCleaningMechanism* targetsCleaner = new TargetsCleaningMechanism(tcPolicy, cleaner);
   mechanism->add(targetsCleaner);

   // background
   if (m_skyBox != NULL)
   {
      SettableRenderingTargetsPolicy* bgPolicy = new SettableRenderingTargetsPolicy();
      bgPolicy->addTarget(0, *m_middleTarget);
      RendererImpl* rendererImpl = m_factory.createFixedRendererImpl();
      GeometryRenderingMechanism* backgroundRenderer = new GeometryRenderingMechanism(bgPolicy, rendererImpl);
      
      backgroundRenderer->setActiveCamera(m_camera);
      backgroundRenderer->setStorage(new demo::SharedRenderablesStorage(*m_skyBox));
      
      backgroundRenderer->setRenderingAction(new RenderMaterial());

      mechanism->add(backgroundRenderer);
   }

   // scene with static objects
   if (m_staticScene != NULL)
   {
      SettableRenderingTargetsPolicy* statScenePolicy = new SettableRenderingTargetsPolicy();
      statScenePolicy->addTarget(0, *m_middleTarget);
      RendererImpl* rendererImpl = m_factory.createFixedRendererImpl();
      GeometryRenderingMechanism* statSceneRenderer = new GeometryRenderingMechanism(statScenePolicy, rendererImpl);

      statSceneRenderer->setActiveCamera(m_camera);

      statSceneRenderer->setStorage(new demo::SharedRenderablesStorage(*m_staticScene));

      RenderingActionsComposite* renderingAction = new RenderingActionsComposite();
      renderingAction->addAction(new RenderMaterial());
      if (m_lights != NULL) {renderingAction->addAction(new RenderLights(*m_lights));}
      statSceneRenderer->setRenderingAction(renderingAction);

      mechanism->add(statSceneRenderer);
   }

   // scene with dynamic objects
   if (m_dynamicScene != NULL)
   {
      SettableRenderingTargetsPolicy* dynScenePolicy = new SettableRenderingTargetsPolicy();
      dynScenePolicy->addTarget(0, *m_middleTarget);
      RendererImpl* rendererImpl = m_factory.createFixedRendererImpl();
      GeometryRenderingMechanism* dynSceneRenderer = new GeometryRenderingMechanism(dynScenePolicy, rendererImpl);

      dynSceneRenderer->setActiveCamera(m_camera);

      dynSceneRenderer->setStorage(new demo::SharedRenderablesStorage(*m_dynamicScene));

      RenderingActionsComposite* renderingAction = new RenderingActionsComposite();
      renderingAction->addAction(new RenderMaterial());
      if (m_lights != NULL) {renderingAction->addAction(new RenderLights(*m_lights));}
      dynSceneRenderer->setRenderingAction(renderingAction);

      mechanism->add(dynSceneRenderer);
   }

   // post process
   if (m_postProcessScene != NULL)
   {
      SettableRenderingTargetsPolicy* ppScenePolicy = new SettableRenderingTargetsPolicy();
      ppScenePolicy->addTarget(0, *m_screenTarget);
      
      GraphicalEffect* effect = m_factory.createEffect(m_rendererImplFile);
      RendererImpl* rendererImpl = m_rendererImplCreator->create(effect);

      GeometryRenderingMechanism* ppSceneRenderer = new GeometryRenderingMechanism(ppScenePolicy, rendererImpl);

      ppSceneRenderer->setActiveCamera(m_camera);
      ppSceneRenderer->setStorage(new demo::SharedRenderablesStorage(*m_postProcessScene));

      RenderingActionsComposite* renderingAction = new RenderingActionsComposite();
      renderingAction->addAction(new RenderMaterial());
      ppSceneRenderer->setRenderingAction(new RenderMaterial());
      mechanism->add(ppSceneRenderer);
   }

   // overlays
   if (m_overlay != NULL)
   {
      SettableRenderingTargetsPolicy* overlayPolicy = new SettableRenderingTargetsPolicy();
      overlayPolicy->addTarget(0, *m_screenTarget);
      OverlayRenderingMechanism* overlayRenderer = new OverlayRenderingMechanism(overlayPolicy);

      overlayRenderer->setOverlay(new demo::SharedOverlay(*m_overlay));
      mechanism->add(overlayRenderer);
   }

   m_renderer.setMechanism(mechanism);
}

///////////////////////////////////////////////////////////////////////////////
