#include "core-Renderer\SceneRenderingMechanism.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\RenderingProcessor.h"
#include "core-Renderer\GraphicalNodesProcessor.h"
#include "core\Frustum.h"


///////////////////////////////////////////////////////////////////////////////

SceneRenderingMechanism::SceneRenderingMechanism(RenderingTargetsPolicy* policy,
                                                 unsigned int maxLightsCount)
      : RenderingMechanism(policy),
      m_maxLightsCount(maxLightsCount),
      m_nodesProcessor(new GraphicalNodesProcessor()),
      m_renderingProcessor(new RenderingProcessor())
{
}

///////////////////////////////////////////////////////////////////////////////

SceneRenderingMechanism::~SceneRenderingMechanism()
{
   m_sceneManagers.clear();

   delete m_nodesProcessor;
   m_nodesProcessor = NULL;

   delete m_renderingProcessor;
   m_renderingProcessor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderingMechanism::addVisualSceneManager(VisualSceneManager& manager)
{
   m_sceneManagers.push_back(&manager);
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderingMechanism::removeVisualSceneManager(VisualSceneManager& manager)
{
   unsigned int idx = m_sceneManagers.find(&manager);
   if (idx != EOA)
   {
      m_sceneManagers.remove(idx);
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderingMechanism::render()
{
   unsigned int managersCount = m_sceneManagers.size();
   for (unsigned int managerIdx = 0; managerIdx < managersCount; ++managerIdx)
   {
      VisualSceneManager& sceneManager = *(m_sceneManagers[managerIdx]);
      if (sceneManager.hasActiveCamera() == false) {continue;}

      Camera& activeCamera = sceneManager.getActiveCamera();

      // set the lights
      const std::list<Light*>& lights = sceneManager.getLights(m_maxLightsCount);
      for (std::list<Light*>::const_iterator it = lights.begin(); 
           it != lights.end(); ++it)
      {
         (*it)->enable(true);
      }

      // set the camera
      const D3DXMATRIX& viewMtx = activeCamera.getViewMtx();
      const D3DXMATRIX& camMtx = activeCamera.getGlobalMtx();
      D3DXVECTOR3 cameraPos(camMtx._41, camMtx._42, camMtx ._43);

      setViewMatrix(viewMtx);
      setProjectionMatrix(activeCamera.getProjectionMtx());

      // render the scene
      renderBackground(sceneManager);
      renderContents(sceneManager, activeCamera);

      // turn the lights off
      for (std::list<Light*>::const_iterator it = lights.begin(); it != lights.end(); ++it)
      {
         (*it)->enable(false);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderingMechanism::renderBackground(VisualSceneManager& sceneManager)
{
   if (sceneManager.isSkyBox() == true) 
   {
      SkyBox& skyBox = sceneManager.getSkyBox();
      skyBox.render();
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderingMechanism::renderContents(VisualSceneManager& sceneManager, 
                                             Camera& activeCamera)
{
   m_visibleNodes.clear();
   m_nodesToProcess.clear();
   
   Frustum cameraFrustum = activeCamera.getFrustum();
   sceneManager.query<AbstractGraphicalNode>(cameraFrustum, m_visibleNodes);
   
   D3DXMATRIX cameraMtx = activeCamera.getGlobalMtx();
   D3DXVECTOR3 cameraPos(cameraMtx._41, cameraMtx._42, cameraMtx._43);

   (*m_nodesProcessor)(m_visibleNodes, cameraFrustum, cameraPos, m_nodesToProcess);

   m_renderingProcessor->translate(m_nodesToProcess);
}

///////////////////////////////////////////////////////////////////////////////
