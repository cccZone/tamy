#include "core-Renderer\VisualSceneManager.h"
#include "core\Node.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\ActiveCameraNode.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Culler.h"
#include "core-Renderer\FrustumCuller.h"
#include "core-Renderer\GraphicalNodesAnalyzer.h"
#include "core-Renderer\OctreeSpatialContainer.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::VisualSceneManager(SpatialContainer* nodesContainer)
      : m_culler(new FrustumCuller()),
      m_activeCameraDeploymentNode(new ActiveCameraNode()),
      m_activeCamera(NULL),
      m_skyBox(NULL),
      m_nodesContainer(NULL)
{
   REGISTER_SCENE_ASPECT(Light);
   REGISTER_SCENE_ASPECT(AbstractGraphicalNode);
   REGISTER_SCENE_ASPECT(Camera);
   REGISTER_SCENE_ASPECT(SkyBox);

   if (nodesContainer != NULL)
   {
      m_nodesContainer = nodesContainer;
   }
   else
   {
      m_nodesContainer = new OctreeSpatialContainer(64, 1000);
   }
}

///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::~VisualSceneManager()
{
   delete m_nodesContainer;
   m_nodesContainer = NULL;

   delete m_culler;
   m_culler = NULL;

   m_visibleNodes.clear();
   m_nodesForSorting.clear();

   delete m_activeCameraDeploymentNode;
   m_activeCameraDeploymentNode = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::setCuller(Culler* culler)
{
   if (culler == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a Culler instance");
   }

   delete m_culler;
   m_culler = culler;
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
   m_nodesContainer->insert(&node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::remove(AbstractGraphicalNode& node)
{
   m_nodesContainer->remove(&node);
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

AbstractGraphicalNodeP* VisualSceneManager::getNodes(DWORD& arraySize)
{
   if (hasActiveCamera() == false) 
   {
      arraySize = 0;
      return NULL;
   }

   Camera& activeCamera = getActiveCamera();

   // get the nodes from the sectors we can see
   m_potentiallyVisibleNodes.clear();
   Frustum frustum = activeCamera.getFrustrum();
   m_nodesContainer->query(frustum, m_potentiallyVisibleNodes);

   // filter the nodes we can't see
   m_culler->setup(frustum, m_visibleNodes);
   GraphicalNodesAnalyzer<Culler> visibilityAnalyzer(*m_culler);

   m_visibleNodes.clear();
   std::for_each((AbstractGraphicalNodeP*)m_potentiallyVisibleNodes, 
      (AbstractGraphicalNodeP*)m_potentiallyVisibleNodes + m_potentiallyVisibleNodes.size(), 
      visibilityAnalyzer);

   // segregate nodes
   m_nodesForSorting.clear();
   unsigned int nodesCount = m_visibleNodes.size();
   unsigned int regularNodesCount = 0;
   for (unsigned int i = 0; i < nodesCount; ++i)
   {
      if (m_visibleNodes[i]->getMaterial().isTransparent() == false)
      {
         m_nodesForSorting.push_back(m_visibleNodes[i]);
         regularNodesCount++;
      }
   }

   for (unsigned int i = 0; i < nodesCount; ++i)
   {
      if (m_visibleNodes[i]->getMaterial().isTransparent() == true)
      {
         m_nodesForSorting.push_back(m_visibleNodes[i]);
      }
   }

   // sort the nodes
   std::sort((AbstractGraphicalNodeP*)m_nodesForSorting, 
      (AbstractGraphicalNodeP*)m_nodesForSorting + regularNodesCount,
      m_materialsComparator);


   D3DXMATRIX mtx = activeCamera.getGlobalMtx();
   m_distanceComparator.setReferencePoint(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
   std::sort((AbstractGraphicalNodeP*)m_nodesForSorting + regularNodesCount, 
      (AbstractGraphicalNodeP*)m_nodesForSorting + m_nodesForSorting.size(),
      m_distanceComparator);


   // output the results
   arraySize = m_nodesForSorting.size();
   return m_nodesForSorting;
}

///////////////////////////////////////////////////////////////////////////////
