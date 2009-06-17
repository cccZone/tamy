#include "core-Renderer\VisualSceneManager.h"
#include "core\Node.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\ActiveCameraNode.h"
#include "core-Renderer\LinearNodesStorage.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::VisualSceneManager(unsigned int maxElemsPerSector, float worldSize)
      : m_activeCameraDeploymentNode(new ActiveCameraNode()),
      m_activeCamera(NULL),
      m_skyBox(NULL),
      m_staticNodesContainer(NULL)
{
   REGISTER_SCENE_ASPECT(Light);
   REGISTER_SCENE_ASPECT(AbstractGraphicalNode);
   REGISTER_SCENE_ASPECT(Camera);
   REGISTER_SCENE_ASPECT(SkyBox);

   m_staticNodesContainer = new Octree<AbstractGraphicalNodeP, AGNVolExtractor, BoundingSphere>(maxElemsPerSector, worldSize);
   m_dynamicNodesContainer = new LinearNodesStorage();
}

///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::~VisualSceneManager()
{
   delete m_dynamicNodesContainer;
   m_dynamicNodesContainer = NULL;

   delete m_staticNodesContainer;
   m_staticNodesContainer = NULL;

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

void VisualSceneManager::add(Light& light)
{
   m_allLights.push_back(&light);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::remove(Light& light)
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

void VisualSceneManager::add(Camera& node)
{
   if (m_activeCamera != NULL) {return;}
   setActiveCamera(node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::remove(Camera& node)
{
   if (m_activeCamera != &node) {return;}
   m_activeCamera = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::add(AbstractGraphicalNode& node) 
{
   if (node.isDynamic())
   {
      m_dynamicNodesContainer->insert(node);
   }
   else
   {
      m_staticNodesContainer->insert(&node);
   }
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::remove(AbstractGraphicalNode& node)
{
   if (node.isDynamic())
   {
      m_dynamicNodesContainer->remove(node);
   }
   else
   {
      m_staticNodesContainer->remove(&node);
   }
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::add(SkyBox& skyBox)
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

void VisualSceneManager::remove(SkyBox& skyBox)
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
