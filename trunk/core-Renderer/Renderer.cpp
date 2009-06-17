#include "core-Renderer\Renderer.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\RenderingPass.h"
#include "core\Point.h"
#include <cassert>


///////////////////////////////////////////////////////////////////////////////

Renderer::Renderer()
      : m_viewportWidth(800),
      m_viewportHeight(600),
      m_leftClientArea(0),
      m_topClientArea(0),
      m_rightClientArea(m_viewportWidth),
      m_bottomClientArea(m_viewportHeight),
      m_initialState(new InitialState()),
      m_renderingState(new RenderingState()),
      m_deviceLostState(new DeviceLostState()),
      m_currentRendererState(m_initialState) // we always start in the initial state
{
}

///////////////////////////////////////////////////////////////////////////////

Renderer::~Renderer()
{
   m_sceneManagers.clear();

   for (unsigned int i = 0; i < m_renderingPasses.size(); ++i)
   {
      delete m_renderingPasses[i];
   }
   m_renderingPasses.clear();

   m_currentRendererState = NULL;

   delete m_initialState;
   m_initialState = NULL;

   delete m_renderingState;
   m_renderingState = NULL;

   delete m_deviceLostState;
   m_deviceLostState = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::addVisualSceneManager(VisualSceneManager& manager)
{
   m_sceneManagers.push_back(&manager);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::removeVisualSceneManager(VisualSceneManager& manager)
{
   for (std::list<VisualSceneManager*>::iterator it = m_sceneManagers.begin();
        it != m_sceneManagers.end(); ++it)
   {
      if (*it == &manager)
      {
         m_sceneManagers.erase(it);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::render()
{
   m_currentRendererState->render(*this);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::InitialState::render(Renderer& renderer)
{
   renderer.initRenderer();
   renderer.resetViewport(renderer.m_viewportWidth, renderer.m_viewportHeight);
   renderer.setRenderingState();

   // we don't want to loose the rendering frame just for rendering - 
   // invoke the rendering method once again so that the actual rendering
   // takes place also
   renderer.render();
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::RenderingState::render(Renderer& renderer)
{
   if (renderer.isGraphicsSystemReady() == false)
   {
      renderer.setDeviceLostState();
      return;
   }

   unsigned int passesCount = renderer.m_renderingPasses.size();
   if (passesCount == 0) {return;}

   renderer.renderingBegin();

   for (std::list<VisualSceneManager*>::iterator it = renderer.m_sceneManagers.begin();
        it != renderer.m_sceneManagers.end(); ++it)
   {
      VisualSceneManager& sceneManager = **it;
      if (sceneManager.hasActiveCamera() == false) {continue;}

      Camera& activeCamera = sceneManager.getActiveCamera();

      // set the lights
      UINT maxLights = renderer.getMaxLightsCount();
      const std::list<Light*>& lights = sceneManager.getLights(maxLights);
      for (std::list<Light*>::const_iterator it = lights.begin(); 
           it != lights.end(); ++it)
      {
         (*it)->enable(true);
      }

      activeCamera.setNearPlaneDimensions(renderer.m_viewportWidth, renderer.m_viewportHeight);
      // set the camera
      const D3DXMATRIX& viewMtx = activeCamera.getViewMtx();
      const D3DXMATRIX& camMtx = activeCamera.getGlobalMtx();
      D3DXVECTOR3 cameraPos(camMtx._41, camMtx._42, camMtx ._43);

      renderer.setViewMatrix(viewMtx);
      renderer.setProjectionMatrix(activeCamera.getProjectionMtx());

      for (unsigned int passIdx = 0; passIdx < passesCount; ++passIdx)
      {
         (*renderer.m_renderingPasses[passIdx])(sceneManager, renderer);
      }

      // turn the lights off
      for (std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); ++it)
      {
         (*it)->enable(false);
      }
   }

   renderer.renderingEnd();
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::DeviceLostState::render(Renderer& renderer)
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

void Renderer::resizeViewport(unsigned int width, unsigned int height,
                              unsigned int leftClientArea, unsigned int topClientArea,
                              unsigned int rightClientArea, unsigned int bottomClientArea)
{
   m_viewportWidth = width;
   m_viewportHeight = height;

   m_leftClientArea = leftClientArea;
   m_topClientArea = topClientArea;
   m_rightClientArea = rightClientArea;
   m_bottomClientArea = bottomClientArea;

   resetViewport(m_viewportWidth, m_viewportHeight);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::addPass(RenderingPass* pass)
{
   if (pass == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderingPass instance");
   }
   m_renderingPasses.push_back(pass);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::screenToViewport(const Point& screenPt, 
                                D3DXVECTOR2& viewportPt) const
{
   float x = (screenPt.x < m_rightClientArea) ? screenPt.x : m_rightClientArea;
   float y = (screenPt.y < m_bottomClientArea) ? screenPt.y : m_bottomClientArea;

   x -= m_leftClientArea;
   y -= m_topClientArea;
   if (x < 0) x = 0;
   if (y < 0) y = 0;

   viewportPt.x = (x / (m_viewportWidth * 0.5f)) - 1;
   viewportPt.y = 1 - (y / (m_viewportHeight * 0.5f));
}

///////////////////////////////////////////////////////////////////////////////
