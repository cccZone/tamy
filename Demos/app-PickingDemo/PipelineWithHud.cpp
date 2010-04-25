#include "PipelineWithHud.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\SortingRenderablesStorage.h"
#include "core-Renderer\LightsStorage.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\CompositeRenderingMechanism.h"
#include "core-Renderer\GeometryRenderingMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "core-Renderer\RenderingActionsComposite.h"
#include "core-Renderer\RenderMaterial.h"
#include "core-Renderer\RenderLights.h"
#include "core-Renderer\StaticGeometryRenderable.h"
#include "core-Renderer\TargetsCleaningMechanism.h"
#include "core-Renderer\OverlayRenderingMechanism.h"
#include "core-Renderer\Overlay.h"
#include "core-Renderer\Renderer.h"
#include "ext-Demo\SharedOverlay.h"
#include "ext-Demo\SharedRenderablesStorage.h"


///////////////////////////////////////////////////////////////////////////////

PipelineWithHud::PipelineWithHud(GraphicalEntitiesFactory& factory,
                                 Renderer& renderer,
                                 Camera& regularCamera,
                                 Camera& hudCamera)
: m_factory(factory)
, m_renderer(renderer)
, m_regularCamera(regularCamera)
, m_hudCamera(hudCamera)
, m_screenTarget(NULL)
, m_regularSceneStorage(NULL)
, m_hudSceneStorage(NULL)
, m_regularScene(NULL)
, m_hudScene(NULL)
, m_regularLights(NULL)
, m_hudLights(NULL)
, m_overlay(NULL)
{
   m_screenTarget = m_factory.createDefaultRenderingTarget();
}

///////////////////////////////////////////////////////////////////////////////

PipelineWithHud::~PipelineWithHud()
{
   delete m_screenTarget;
   m_screenTarget = NULL;

   delete m_regularSceneStorage;
   m_regularSceneStorage = NULL;

   delete m_hudSceneStorage;
   m_hudSceneStorage = NULL;

   delete m_regularScene;
   m_regularScene = NULL;

   delete m_hudScene;
   m_hudScene = NULL;

   delete m_regularLights;
   m_regularLights = NULL;

   delete m_hudLights;
   m_hudLights = NULL;

   delete m_overlay;
   m_overlay = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PipelineWithHud::setRegularScene(SpatialStorage<RenderableNode>* scene)
{
   delete m_regularSceneStorage;
   delete m_regularScene;
   
   m_regularSceneStorage = scene;
   if (m_regularSceneStorage != NULL)
   {
      m_regularScene = new SortingRenderablesStorage<RenderableNode>(*m_regularSceneStorage);
   }
   else
   {
      m_regularScene = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void PipelineWithHud::setHudScene(SpatialStorage<RenderableNode>* scene)
{
   delete m_hudSceneStorage;
   delete m_hudScene;
   
   m_hudSceneStorage = scene;
   if (m_hudSceneStorage != NULL)
   {
      m_hudScene = new SortingRenderablesStorage<RenderableNode>(*m_hudSceneStorage);
   }
   else
   {
      m_hudScene = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void PipelineWithHud::setRegularLights(LightsStorage* lights)
{
   delete m_regularLights;
   m_regularLights = lights;
}

///////////////////////////////////////////////////////////////////////////////

void PipelineWithHud::setHudLights(LightsStorage* lights)
{
   delete m_hudLights;
   m_hudLights = lights;
}

///////////////////////////////////////////////////////////////////////////////

void PipelineWithHud::setOverlay(Overlay* overlay)
{
   delete m_overlay;
   m_overlay = overlay;
}

///////////////////////////////////////////////////////////////////////////////

void PipelineWithHud::update(float timeElapsed)
{
   m_renderer.render();
}

///////////////////////////////////////////////////////////////////////////////

void PipelineWithHud::create()
{
   CompositeRenderingMechanism* mechanism = new CompositeRenderingMechanism();

   // targets cleaner
   SettableRenderingTargetsPolicy* tcPolicy = new SettableRenderingTargetsPolicy();
   tcPolicy->addTarget(0, *m_screenTarget);
   RenderingTargetsCleaner* cleaner = m_factory.createTargetsCleaner();
   TargetsCleaningMechanism* targetsCleaner = new TargetsCleaningMechanism(tcPolicy, cleaner);
   mechanism->add(targetsCleaner);

   // regular scene
   if (m_regularScene != NULL)
   {
      SettableRenderingTargetsPolicy* regScenePolicy = new SettableRenderingTargetsPolicy();
      regScenePolicy->addTarget(0, *m_screenTarget);
      RendererImpl* rendererImpl = m_factory.createFixedRendererImpl();
      GeometryRenderingMechanism* regSceneRenderer = new GeometryRenderingMechanism(regScenePolicy, rendererImpl);

      regSceneRenderer->setActiveCamera(m_regularCamera);
      regSceneRenderer->setStorage(new demo::SharedRenderablesStorage(*m_regularScene));

      RenderingActionsComposite* renderingAction = new RenderingActionsComposite();
      renderingAction->addAction(new RenderMaterial());
      if (m_regularLights != NULL) {renderingAction->addAction(new RenderLights(*m_regularLights));}
      regSceneRenderer->setRenderingAction(renderingAction);

      mechanism->add(regSceneRenderer);
   }

   // hud scene
   if (m_hudScene != NULL)
   {
      SettableRenderingTargetsPolicy* hudScenePolicy = new SettableRenderingTargetsPolicy();
      hudScenePolicy->addTarget(0, *m_screenTarget);
      RendererImpl* rendererImpl = m_factory.createFixedRendererImpl();
      GeometryRenderingMechanism* hudSceneRenderer = new GeometryRenderingMechanism(hudScenePolicy, rendererImpl);

      hudSceneRenderer->setActiveCamera(m_hudCamera);
      hudSceneRenderer->setStorage(new demo::SharedRenderablesStorage(*m_hudScene));

      RenderingActionsComposite* renderingAction = new RenderingActionsComposite();
      renderingAction->addAction(new RenderMaterial());
      if (m_hudLights != NULL) {renderingAction->addAction(new RenderLights(*m_hudLights));}
      hudSceneRenderer->setRenderingAction(renderingAction);

      mechanism->add(hudSceneRenderer);
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
