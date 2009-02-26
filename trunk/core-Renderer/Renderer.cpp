#include "Renderer.h"
#include "GraphicalNodesAggregator.h"
#include "LightsAggregator.h"
#include "Camera.h"
#include "Light.h"
#include "RenderingProcessor.h"
#include "SkyBox.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

Renderer::Renderer()
      : m_renderingProcessor(new RenderingProcessor()),
      m_activeCamera(NULL),
      m_skyBox(NULL),
      m_viewportWidth(800),
      m_viewportHeight(600),
      m_initialState(new InitialState()),
      m_renderingState(new RenderingState()),
      m_deviceLostState(new DeviceLostState()),
      m_currentRendererState(m_initialState) // we always start in the initial state
{
}

///////////////////////////////////////////////////////////////////////////////

Renderer::~Renderer()
{
   delete m_renderingProcessor;
   m_renderingProcessor = NULL;

   m_currentRendererState = NULL;

   delete m_initialState;
   m_initialState = NULL;

   delete m_renderingState;
   m_renderingState = NULL;

   delete m_deviceLostState;
   m_deviceLostState = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::render(Node& rootNode)
{
   m_currentRendererState->render(*this, rootNode);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::InitialState::render(Renderer& renderer, Node& rootNode)
{
   renderer.initRenderer();
   renderer.resetViewport(renderer.m_viewportWidth, renderer.m_viewportHeight);
   renderer.setRenderingState();

   // we don't want to loose the rendering frame just for rendering - 
   // invoke the rendering method once again so that the actual rendering
   // takes place also
   renderer.render(rootNode);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::RenderingState::render(Renderer& renderer, Node& rootNode)
{
   if (renderer.isGraphicsSystemReady() == false)
   {
      renderer.setDeviceLostState();
      return;
   }

   assert(renderer.m_renderingProcessor != NULL);
   if (renderer.m_activeCamera == NULL) {return;}



   // set the lights
   LightsAggregator lights;
   rootNode.accept(lights);
   UINT lightIdx = 0;
   UINT maxLights = renderer.getMaxLightsCount();
   for (std::list<Light*>::const_iterator it = lights().begin(); 
        (it != lights().end()) && (lightIdx < maxLights); 
        ++it, ++lightIdx)
   {
      (*it)->enable(true);
   }

   renderer.setViewMatrix(renderer.m_activeCamera->getViewMtx());
   renderer.setProjectionMatrix(renderer.m_activeCamera->getProjectionMtx3D());

   // add the objects to be rendered
   GraphicalNodesAggregator nodes;
   rootNode.accept(nodes);

   // prepare the rendering commands
   std::list<RenderingCommand> renderingCommands = renderer.m_renderingProcessor->translate(nodes());
   renderer.addBackgroundRenderingCommand(renderingCommands);

   renderer.executeRenderingCommands(renderingCommands);

   // render
   renderer.present();

   // turn the lights off
   for (std::list<Light*>::const_iterator it = lights().begin(); it != lights().end(); ++it)
   {
      (*it)->enable(false);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::DeviceLostState::render(Renderer& renderer, Node& rootNode)
{
   renderer.attemptToRecoverGraphicsSystem();

   if (renderer.isGraphicsSystemReady() == true) {renderer.setInitialState();}
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::setInitialState()
{
   m_currentRendererState = m_initialState;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::setRenderingState()
{
   m_currentRendererState = m_renderingState;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::setDeviceLostState()
{
   m_currentRendererState = m_deviceLostState;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::resizeViewport(unsigned int width, unsigned int height)
{
   m_viewportWidth = width;
   m_viewportHeight = height;

   resetActiveCameraSettings();
   resetViewport(m_viewportWidth, m_viewportHeight);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::setActiveCamera(Camera& camera)
{
   m_activeCamera = &camera; 
   resetActiveCameraSettings();
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::setSkyBox(SkyBox& skyBox)
{
   m_skyBox = &skyBox;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::resetActiveCameraSettings()
{
   if (m_activeCamera == NULL) {return;}

   m_activeCamera->setAspectRatio((float)m_viewportWidth / (float)m_viewportHeight);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::addBackgroundRenderingCommand(std::list<RenderingCommand>& renderingCommands)
{
   if (m_skyBox == NULL) {return;}

   m_skyBox->updateOrientation(m_activeCamera->getGlobalMtx());
   renderingCommands.push_front(RenderingCommand::from_method<SkyBox, &SkyBox::render> (m_skyBox));
}

///////////////////////////////////////////////////////////////////////////////
