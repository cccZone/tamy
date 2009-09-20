#include "ext-Demo\BasicRenderingPipeline.h"
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
#include "core-Renderer\Renderer.h"
#include "ext-Demo\StaticSceneManager.h"
#include "ext-Demo\SharedOverlay.h"
#include "ext-Demo\SharedRenderablesStorage.h"
#include "ext-Demo\RERCreator.h"


///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

BasicRenderingPipeline::BasicRenderingPipeline(GraphicalEntitiesFactory& factory,
                                               Renderer& renderer,
                                               Camera& camera)
: m_factory(factory)
, m_renderer(renderer)
, m_camera(camera)
, m_screenTarget(NULL)
, m_skyBox(NULL)
, m_staticSceneStorage(NULL)
, m_dynamicSceneStorage(NULL)
, m_staticScene(NULL)
, m_dynamicScene(NULL)
, m_lights(NULL)
, m_overlay(NULL)
, m_rendererImplCreator(NULL)
{
   m_screenTarget = m_factory.createDefaultRenderingTarget();
}

///////////////////////////////////////////////////////////////////////////////

BasicRenderingPipeline::~BasicRenderingPipeline()
{
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

   delete m_lights;
   m_lights = NULL;

   delete m_overlay;
   m_overlay = NULL;

   delete m_rendererImplCreator;
   m_rendererImplCreator = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void BasicRenderingPipeline::setBackground(RenderablesStorage* bg)
{
   delete m_skyBox;
   m_skyBox = bg;
}

///////////////////////////////////////////////////////////////////////////////

void BasicRenderingPipeline::setStaticScene(SpatialStorage<StaticGeometryRenderable>* stat)
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

void BasicRenderingPipeline::setDynamicScene(SpatialStorage<RenderableNode>* dyn)
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

void BasicRenderingPipeline::setLights(LightsStorage* lights)
{
   delete m_lights;
   m_lights = lights;
}

///////////////////////////////////////////////////////////////////////////////

void BasicRenderingPipeline::setOverlay(Overlay* overlay)
{
   delete m_overlay;
   m_overlay = overlay;
}

///////////////////////////////////////////////////////////////////////////////

void BasicRenderingPipeline::setRendererImpl(const std::string& effectFileName, 
                                             RERCreator* creator)
{
   delete m_rendererImplCreator;
   m_rendererImplCreator = creator;
   m_rendererImplFile = effectFileName;
}

///////////////////////////////////////////////////////////////////////////////

void BasicRenderingPipeline::update(float timeElapsed)
{
   m_renderer.render();
}

///////////////////////////////////////////////////////////////////////////////

void BasicRenderingPipeline::create()
{
   CompositeRenderingMechanism* mechanism = new CompositeRenderingMechanism();

   // targets cleaner
   SettableRenderingTargetsPolicy* tcPolicy = new SettableRenderingTargetsPolicy();
   tcPolicy->addTarget(0, *m_screenTarget);
   RenderingTargetsCleaner* cleaner = m_factory.createTargetsCleaner();
   TargetsCleaningMechanism* targetsCleaner = new TargetsCleaningMechanism(tcPolicy, cleaner);
   mechanism->add(targetsCleaner);

   // background
   if (m_skyBox != NULL)
   {
      SettableRenderingTargetsPolicy* bgPolicy = new SettableRenderingTargetsPolicy();
      bgPolicy->addTarget(0, *m_screenTarget);
      GeometryRenderingMechanism* backgroundRenderer = new GeometryRenderingMechanism(bgPolicy, createRendererImpl());
      
      backgroundRenderer->setActiveCamera(m_camera);
      backgroundRenderer->setStorage(new SharedRenderablesStorage(*m_skyBox));
      
      backgroundRenderer->setRenderingAction(new RenderMaterial());

      mechanism->add(backgroundRenderer);
   }

   // scene with static objects
   if (m_staticScene != NULL)
   {
      SettableRenderingTargetsPolicy* statScenePolicy = new SettableRenderingTargetsPolicy();
      statScenePolicy->addTarget(0, *m_screenTarget);
      GeometryRenderingMechanism* statSceneRenderer = new GeometryRenderingMechanism(statScenePolicy, createRendererImpl());

      statSceneRenderer->setActiveCamera(m_camera);

      statSceneRenderer->setStorage(new SharedRenderablesStorage(*m_staticScene));

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
      dynScenePolicy->addTarget(0, *m_screenTarget);
      GeometryRenderingMechanism* dynSceneRenderer = new GeometryRenderingMechanism(dynScenePolicy, createRendererImpl());

      dynSceneRenderer->setActiveCamera(m_camera);

      dynSceneRenderer->setStorage(new SharedRenderablesStorage(*m_dynamicScene));

      RenderingActionsComposite* renderingAction = new RenderingActionsComposite();
      renderingAction->addAction(new RenderMaterial());
      if (m_lights != NULL) {renderingAction->addAction(new RenderLights(*m_lights));}
      dynSceneRenderer->setRenderingAction(renderingAction);

      mechanism->add(dynSceneRenderer);
   }

   // overlays
   if (m_overlay != NULL)
   {
      SettableRenderingTargetsPolicy* overlayPolicy = new SettableRenderingTargetsPolicy();
      overlayPolicy->addTarget(0, *m_screenTarget);
      OverlayRenderingMechanism* overlayRenderer = new OverlayRenderingMechanism(overlayPolicy);

      overlayRenderer->setOverlay(new SharedOverlay(*m_overlay));
      mechanism->add(overlayRenderer);
   }

   m_renderer.setMechanism(mechanism);
}

///////////////////////////////////////////////////////////////////////////////

RendererImpl* BasicRenderingPipeline::createRendererImpl()
{
   RendererImpl* rendererImpl = NULL;
   
   if (m_rendererImplCreator == NULL)
   {
      rendererImpl = m_factory.createFixedRendererImpl();
   }
   else
   {
      GraphicalEffect* effect = m_factory.createEffect(m_rendererImplFile);
      rendererImpl = m_rendererImplCreator->create(effect);
   }

   if (rendererImpl == NULL)
   {
      throw std::runtime_error("Renderer implemetation not created");
   }

   return rendererImpl;
}

///////////////////////////////////////////////////////////////////////////////

} // namespace demo
