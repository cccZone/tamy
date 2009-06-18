#include "core-Renderer\VisualSceneManager.h"
#include "core\Node.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\ActiveCameraNode.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::VisualSceneManager(unsigned int maxElemsPerSector, float worldSize)
      : SceneAspectManager<Light>(maxElemsPerSector, worldSize),
      SceneAspectManager<AbstractGraphicalNode>(maxElemsPerSector, worldSize),
      SceneAspectManager<Camera>(maxElemsPerSector, worldSize),
      SceneAspectManager<SkyBox>(maxElemsPerSector, worldSize),
      m_activeCameraDeploymentNode(new ActiveCameraNode()),
      m_activeCamera(NULL),
      m_skyBox(NULL)
{
   REGISTER_SCENE_ASPECT(Light);
   REGISTER_SCENE_ASPECT(AbstractGraphicalNode);
   REGISTER_SCENE_ASPECT(Camera);
   REGISTER_SCENE_ASPECT(SkyBox);
}

///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::~VisualSceneManager()
{
   delete m_activeCameraDeploymentNode;
   m_activeCameraDeploymentNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////

const std::list<Light*>& VisualSceneManager::getLights(int lightLimit)
{
   Camera& activeCamera = getActiveCamera();

   bool dirty = (m_currentlyVisibleLights.size() == 0) || 
      (activeCamera.getPosition() != m_cachedCameraPos) ||
      (m_currentlyVisibleLights.size() > lightLimit);

   if (dirty)
   {
      m_cachedCameraPos = activeCamera.getPosition();
      refreshVisibleLights(lightLimit);
   }

   return m_currentlyVisibleLights;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::onAdd(Light& light)
{
   m_allLights.push_back(&light);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::onRemove(Light& light)
{
   for (std::list<Light*>::iterator it = m_allLights.begin();
      it != m_allLights.end(); ++it)
   {
      if (*it == &light) 
      {
         m_allLights.erase(it);
         return;
      }
   }

   throw std::runtime_error("Trying to remove an unregistered Light");
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::refreshVisibleLights(int lightLimit)
{
   m_currentlyVisibleLights.clear();

   int lightIdx = 0;
   for (std::list<Light*>::iterator it = m_allLights.begin(); 
      (it != m_allLights.end()) && (lightIdx < lightLimit); ++it, ++lightIdx)
   {
      m_currentlyVisibleLights.push_back(*it);
   }
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::onAdd(Camera& node)
{
   if (m_activeCamera != NULL) {return;}
   setActiveCamera(node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::onRemove(Camera& node)
{
   if (m_activeCamera != &node) {return;}
   m_activeCamera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::onAdd(AbstractGraphicalNode& node) 
{
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::onRemove(AbstractGraphicalNode& node)
{
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::onAdd(SkyBox& skyBox)
{
   if (m_skyBox != NULL)
   {
      m_activeCameraDeploymentNode->removeChild(*m_skyBox);
      delete m_skyBox;
   }
   m_activeCameraDeploymentNode->addChild(&skyBox);
   m_skyBox = &skyBox;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::onRemove(SkyBox& skyBox)
{
   if (m_skyBox == NULL) {return;}
   if (&skyBox != m_skyBox) {throw std::runtime_error("Trying to remove the invalid skybox");}

   m_activeCameraDeploymentNode->removeChild(*m_skyBox);
   m_skyBox = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::setActiveCamera(Camera& camera)
{
   m_activeCamera = &camera;
   m_activeCameraDeploymentNode->setCameraNode(camera);
}

///////////////////////////////////////////////////////////////////////////////
