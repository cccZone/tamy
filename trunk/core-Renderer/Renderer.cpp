#include "core-Renderer\Renderer.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\SceneManager.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

Renderer::Renderer()
      : m_activeSceneManager(NULL),
      m_renderingProcessor(new RenderingProcessor()),
      m_activeCamera(NULL),
      m_viewportWidth(800),
      m_viewportHeight(600),
      m_initialState(new InitialState()),
      m_renderingState(new RenderingState()),
      m_deviceLostState(new DeviceLostState()),
      m_currentRendererState(m_initialState) // we always start in the initial state
{
   m_commandsArraySize = 100000; // TODO: rozmiar tego
   m_renderingCommands = new RenderingCommand[m_commandsArraySize];

   // configure the rendering pipeline
   m_renderingPasses.push_back(RenderingCommand::from_method<Renderer, &Renderer::renderBackground> (this));
   m_renderingPasses.push_back(RenderingCommand::from_method<Renderer, &Renderer::renderRegular> (this));
   m_renderingPasses.push_back(RenderingCommand::from_method<Renderer, &Renderer::renderTransparent> (this));
}

///////////////////////////////////////////////////////////////////////////////

Renderer::~Renderer()
{
   delete [] m_renderingCommands; 
   m_renderingCommands = NULL;
   m_commandsArraySize = 0;

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

void Renderer::render(SceneManager& sceneManager)
{
   m_currentRendererState->render(*this, sceneManager);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::InitialState::render(Renderer& renderer, SceneManager& sceneManager)
{
   renderer.initRenderer();
   renderer.resetViewport(renderer.m_viewportWidth, renderer.m_viewportHeight);
   renderer.setRenderingState();

   // we don't want to loose the rendering frame just for rendering - 
   // invoke the rendering method once again so that the actual rendering
   // takes place also
   renderer.render(sceneManager);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::RenderingState::render(Renderer& renderer, SceneManager& sceneManager)
{
   if (renderer.isGraphicsSystemReady() == false)
   {
      renderer.setDeviceLostState();
      return;
   }

   assert(renderer.m_renderingProcessor != NULL);
   if (renderer.m_activeCamera == NULL) {return;}

   // set the camera
   const D3DXMATRIX& viewMtx = renderer.m_activeCamera->getViewMtx();
   const D3DXMATRIX& camMtx = renderer.m_activeCamera->getGlobalMtx();
   D3DXVECTOR3 cameraPos(camMtx._41, camMtx._42, camMtx ._43);

   renderer.setViewMatrix(viewMtx);
   renderer.setProjectionMatrix(renderer.m_activeCamera->getProjectionMtx3D());

   // set the lights
   UINT maxLights = renderer.getMaxLightsCount();
   const std::list<Light*>& lights = sceneManager.getLights(*(renderer.m_activeCamera), maxLights);
   for (std::list<Light*>::const_iterator it = lights.begin(); 
        it != lights.end(); ++it)
   {
      (*it)->enable(true);
   }

   // render
   renderer.renderingBegin();
   renderer.m_activeSceneManager = &sceneManager;
   unsigned int passesCount = renderer.m_renderingPasses.size();
   for (unsigned int passIdx = 0; passIdx < passesCount; ++passIdx)
   {
      (renderer.m_renderingPasses[passIdx])();
   }

   renderer.renderingEnd();

   // turn the lights off
   for (std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); ++it)
   {
      (*it)->enable(false);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::renderBackground()
{
   if (m_activeSceneManager->isSkyBox() == false) {return;}

   SkyBox& skyBox = m_activeSceneManager->getSkyBox();
   skyBox.updateOrientation(m_activeCamera->getGlobalMtx());
   skyBox.render();
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::renderRegular()
{
   DWORD nodesArraySize = 0;
   AbstractGraphicalNodeP* nodes = m_activeSceneManager->getRegularGraphicalNodes(
                                                *m_activeCamera, 
                                                 nodesArraySize);

   DWORD commandsCount = m_renderingProcessor->translate(nodes, nodesArraySize, 
                                                         m_renderingCommands, 
                                                         m_commandsArraySize);

   for (DWORD i = 0; i < commandsCount; ++i)
   {
      (m_renderingCommands[i])();
   }
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::renderTransparent()
{
   DWORD nodesArraySize = 0;
   AbstractGraphicalNodeP* nodes = m_activeSceneManager->getTransparentGraphicalNodes(
                                                    *(m_activeCamera), 
                                                     nodesArraySize);

   DWORD commandsCount = m_renderingProcessor->translate(nodes, nodesArraySize, 
                                                         m_renderingCommands, 
                                                         m_commandsArraySize);
   for (DWORD i = 0; i < commandsCount; ++i)
   {
      (m_renderingCommands[i])();
   }
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::DeviceLostState::render(Renderer& renderer, SceneManager& sceneManager)
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

void Renderer::resetActiveCameraSettings()
{
   if (m_activeCamera == NULL) {return;}

   m_activeCamera->setAspectRatio((float)m_viewportWidth / (float)m_viewportHeight);
}

///////////////////////////////////////////////////////////////////////////////
