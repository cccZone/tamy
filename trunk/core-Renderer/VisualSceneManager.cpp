#include "core-Renderer\VisualSceneManager.h"
#include "core\Node.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\ActiveCameraNode.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Culler.h"
#include "core-Renderer\FrustumCuller.h"
#include "core-Renderer\GraphicalNodesAnalyzer.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::VisualSceneManager()
      : m_addOperation(NULL),
      m_removeOperation(NULL),
      m_noOperation(new NoOperation()),
      m_currentOperation(m_noOperation),
      m_culler(new FrustumCuller()),
      m_activeCameraDeploymentNode(new ActiveCameraNode()),
      m_activeCamera(NULL),
      m_skyBox(NULL),
      m_regularNodesTree(64, 1000)
{
   m_addOperation = new AddOperation(*this);
   m_removeOperation = new RemoveOperation(*this);
}

///////////////////////////////////////////////////////////////////////////////

VisualSceneManager::~VisualSceneManager()
{
   delete m_culler;
   m_culler = NULL;

   m_regularRenderingQueue.clear();

   DWORD nodesCount = m_transparentNodes.size();
   for (DWORD i = 0; i < nodesCount; ++i)
   {
      delete m_transparentNodes[i];
   }
   m_transparentNodes.clear();
   m_transparentRenderingQueue.clear();

   delete m_activeCameraDeploymentNode;
   m_activeCameraDeploymentNode = NULL;

   m_currentOperation = NULL;

   delete m_noOperation;
   m_noOperation = NULL;

   delete m_removeOperation;
   m_removeOperation = NULL;

   delete m_addOperation;
   m_addOperation = NULL;
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

void VisualSceneManager::addNode(Node* node)
{
   m_currentOperation = m_addOperation;
   node->accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::removeNode(Node& node)
{
   m_currentOperation = m_removeOperation;
   node.accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::visit(Light& light)
{
   m_currentOperation->perform(light);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::visit(AbstractGraphicalNode& node)
{
   m_currentOperation->perform(node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::visit(Camera& node)
{
   m_currentOperation->perform(node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::visit(SkyBox& node)
{
   m_currentOperation->perform(node);
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
   if (node.getMaterial().isTransparent())
   {
      addTransparentNode(node);
   }
   else
   {
      addRegularNode(node);  
   }
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::remove(AbstractGraphicalNode& node)
{
   if (node.getMaterial().isTransparent())
   {
      removeTransparentNode(node);
   }
   else
   {
      removeRegularNode(node);
   }
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::setSkyBox(SkyBox* skyBox)
{
   if (m_skyBox != NULL)
   {
      m_activeCameraDeploymentNode->removeChild(*m_skyBox);
      delete m_skyBox;
   }
   m_activeCameraDeploymentNode->addChild(skyBox);
   m_skyBox = skyBox;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::setActiveCamera(Camera& camera)
{
   m_activeCamera = &camera;
   m_activeCameraDeploymentNode->setCameraNode(camera);
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalNodeP* VisualSceneManager::getRegularGraphicalNodes(DWORD& arraySize)
{
   if (hasActiveCamera() == false) 
   {
      arraySize = 0;
      return NULL;
   }

   // get the nodes in the visible sector
   m_regularGraphicalNodes.clear();
   Frustum frustum = getActiveCamera().getFrustrum();
   m_regularNodesTree.query(frustum, m_regularGraphicalNodes);

   // create a list of visible nodes
   m_regularRenderingQueue.clear();
   m_culler->setup(getActiveCamera(), m_regularRenderingQueue);
   GraphicalNodesAnalyzer<Culler> analyzer(*m_culler);

   std::for_each((AbstractGraphicalNodeP*)m_regularGraphicalNodes, 
                 (AbstractGraphicalNodeP*)m_regularGraphicalNodes + m_regularGraphicalNodes.size(), 
                 analyzer);

   // sort the nodes
   std::sort((AbstractGraphicalNodeP*)m_regularRenderingQueue, 
      (AbstractGraphicalNodeP*)m_regularRenderingQueue + m_regularRenderingQueue.size(),
      m_materialsComparator);
   
   arraySize = m_regularRenderingQueue.size();
   return m_regularRenderingQueue;
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalNodeP* VisualSceneManager::getTransparentGraphicalNodes(DWORD& arraySize)
{
   if ((hasActiveCamera() == false) || (m_transparentNodes.size() == 0))
   {
      arraySize = 0;
      return NULL;
   }

   // create a list of visible nodes
   m_transparentRenderingQueue.clear();

   m_culler->setup(getActiveCamera(), m_transparentRenderingQueue);
   GraphicalNodesAnalyzer<Culler> analyzer(*m_culler);

   std::for_each((AbstractGraphicalNodeP*)m_transparentNodes, 
                 (AbstractGraphicalNodeP*)m_transparentNodes + m_transparentNodes.size(), 
                 analyzer);

   // sort the nodes
   Camera& activeCamera = getActiveCamera();
   D3DXMATRIX mtx = activeCamera.getGlobalMtx();

   m_distanceComparator.setReferencePoint(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
   std::sort((AbstractGraphicalNodeP*)m_transparentRenderingQueue, 
             (AbstractGraphicalNodeP*)m_transparentRenderingQueue + m_transparentRenderingQueue.size(),
             m_distanceComparator);

   arraySize = m_transparentRenderingQueue.size();
   return m_transparentRenderingQueue;
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::addRegularNode(AbstractGraphicalNode& node)
{
   m_regularNodesTree.insert(&node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::removeRegularNode(AbstractGraphicalNode& node)
{
   m_regularNodesTree.remove(&node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::addTransparentNode(AbstractGraphicalNode& node)
{
   m_transparentNodes.push_back(&node);
}

///////////////////////////////////////////////////////////////////////////////

void VisualSceneManager::removeTransparentNode(AbstractGraphicalNode& node)
{
   unsigned int pos = m_transparentNodes.find(&node);
   m_transparentNodes.remove(pos);
}

///////////////////////////////////////////////////////////////////////////////
