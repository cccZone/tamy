#include "core-Renderer\Renderer.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\RenderingPass.h"
#include "core-Renderer\RenderingTarget.h"
#include "core\Point.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

Renderer::Renderer(unsigned int maxRenderingTargets)
      : m_renderingTargets(maxRenderingTargets),
      m_maxRenderingTargets(maxRenderingTargets),
      m_viewportWidth(800),
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
   if (m_maxRenderingTargets == 0)
   {
      throw std::invalid_argument("Graphics device has to allow for at least one rendering target");
   }
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
   unsigned int idx = m_sceneManagers.find(&manager);
   if (idx != EOA)
   {
      m_sceneManagers.remove(idx);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::addRenderingTarget(RenderingTarget& target)
{
   if (m_renderingTargets.size() >= m_maxRenderingTargets)
   {
      throw std::runtime_error("Too many rendering targets for your graphics device");
   }

   m_renderingTargets.push_back(&target);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::removeRenderingTarget(RenderingTarget& target)
{
   unsigned int idx = m_renderingTargets.find(&target);
   if (idx != EOA)
   {
      m_renderingTargets.remove(idx);
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

   // check if there are any rendering passes - none mean that 
   // we have noting to render with
   unsigned int passesCount = renderer.m_renderingPasses.size();
   if (passesCount == 0) {return;}

   // set the rendering targets
   unsigned int rtsCount = renderer.m_renderingTargets.size();
   if (rtsCount == 0) {return;}
   for (unsigned int i = 0; i < rtsCount; ++i)
   {
      renderer.m_renderingTargets[i]->use(i);
   }

   // begin rendering 
   renderer.renderingBegin();

   unsigned int managersCount = renderer.m_sceneManagers.size();
   for (unsigned int managerIdx = 0; managerIdx < managersCount; ++managerIdx)
   {
      VisualSceneManager& sceneManager = *(renderer.m_sceneManagers[managerIdx]);
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

      activeCamera.setNearPlaneDimensions((float)(renderer.m_viewportWidth), 
                                          (float)(renderer.m_viewportHeight));
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
