#include "core-Renderer\BasicSceneManager.h"
#include "core\Node.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Camera.h"
#include <windows.h>
#include <math.h>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BasicSceneManager::BasicSceneManager()
{
   m_regularRenderingQueueSize = 0;
   m_regularRenderingQueue = NULL;

   m_transparentRenderingQueueSize = 1;
   m_transparentNodesCount = 0;
   m_transparentRenderingQueue = new AbstractGraphicalNodeP[m_transparentRenderingQueueSize];
   m_transparentRenderingQueue[0] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

BasicSceneManager::~BasicSceneManager()
{
   m_regularRenderingQueueSize = 0;
   delete [] m_regularRenderingQueue;
   m_regularRenderingQueue = NULL;

   m_transparentRenderingQueueSize = 0;
   delete [] m_transparentRenderingQueue;
   m_transparentRenderingQueue = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void BasicSceneManager::addToHierarchy(Node* newNode)
{
   getRootNode().addChild(newNode);
}

///////////////////////////////////////////////////////////////////////////////

void BasicSceneManager::removeFromHierarchy(Node* node) 
{
   getRootNode().removeChild(*node);
}

///////////////////////////////////////////////////////////////////////////////

const std::list<Light*>& BasicSceneManager::getLights(int lightLimit)
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

AbstractGraphicalNodeP* BasicSceneManager::getRegularGraphicalNodes(DWORD& arraySize)
{
   if (hasActiveCamera() == false) 
   {
      arraySize = 0;
      return NULL;
   }

   if (m_regularGraphicalNodes.size() != m_regularRenderingQueueSize)
   {
      delete [] m_regularRenderingQueue;

      m_regularRenderingQueueSize = m_regularGraphicalNodes.size();
      m_regularRenderingQueue = new AbstractGraphicalNodeP[m_regularRenderingQueueSize];

      DWORD i = 0;
      for (GraphicalNodesSet::iterator it = m_regularGraphicalNodes.begin();
           it != m_regularGraphicalNodes.end(); ++it, ++i)
      {
         m_regularRenderingQueue[i] = *it;
      }
   }

   arraySize = m_regularRenderingQueueSize;
   return m_regularRenderingQueue;
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalNodeP* BasicSceneManager::getTransparentGraphicalNodes(DWORD& arraySize)
{
   if (hasActiveCamera() == false) 
   {
      arraySize = 0;
      return NULL;
   }

   if (m_transparentNodesCount > 0)
   {
      Camera& activeCamera = getActiveCamera();
      m_distanceComparator.setReferencePoint(activeCamera.getPosition());
      std::sort(m_transparentRenderingQueue, 
                m_transparentRenderingQueue + m_transparentNodesCount,
                m_distanceComparator);
   }

   arraySize = m_transparentNodesCount;
   return m_transparentRenderingQueue;
}

///////////////////////////////////////////////////////////////////////////////

void BasicSceneManager::add(Light& light)
{
   m_allLights.push_back(&light);
}

///////////////////////////////////////////////////////////////////////////////

void BasicSceneManager::remove(Light& light)
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

void BasicSceneManager::add(AbstractGraphicalNode& node) 
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

void BasicSceneManager::remove(AbstractGraphicalNode& node)
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

void BasicSceneManager::addRegularNode(AbstractGraphicalNode& node)
{
   m_regularGraphicalNodes.insert(&node);  
}

///////////////////////////////////////////////////////////////////////////////

void BasicSceneManager::removeRegularNode(AbstractGraphicalNode& node)
{
   GraphicalNodesSet::iterator it = m_regularGraphicalNodes.begin(); 
   for (; it != m_regularGraphicalNodes.end(); ++it)
   {
      if (*it == &node) {break;}
   }
   if (it == m_regularGraphicalNodes.end())
   {
      throw std::runtime_error("Trying to remove an unregistered regular AbstractGraphicalNode");
   }
   m_regularGraphicalNodes.erase(it);
}

///////////////////////////////////////////////////////////////////////////////

void BasicSceneManager::addTransparentNode(AbstractGraphicalNode& node)
{
   if (m_transparentNodesCount + 1 >  m_transparentRenderingQueueSize)
   {
      DWORD newSize = m_transparentRenderingQueueSize * 2;
      AbstractGraphicalNodeP* tmpArr = new AbstractGraphicalNodeP[newSize];
      ZeroMemory(tmpArr, sizeof(AbstractGraphicalNodeP) * newSize);

      memcpy(tmpArr, m_transparentRenderingQueue, 
             sizeof(AbstractGraphicalNodeP) * m_transparentRenderingQueueSize);
      delete [] m_transparentRenderingQueue;

      m_transparentRenderingQueueSize = newSize;
      m_transparentRenderingQueue = tmpArr;
   }

   m_transparentRenderingQueue[m_transparentNodesCount++] = &node;
}

///////////////////////////////////////////////////////////////////////////////

void BasicSceneManager::removeTransparentNode(AbstractGraphicalNode& node)
{
   // find the node
   unsigned int nodeIdx = 0;
   for (; nodeIdx < m_transparentNodesCount; ++nodeIdx)
   {
      if (m_transparentRenderingQueue[nodeIdx] == &node)
      {
         break;
      }
   }

   if (nodeIdx >= m_transparentNodesCount)
   {
      throw std::runtime_error("Trying to remove an unregistered transparent AbstractGraphicalNode");
   }

   memcpy(m_transparentRenderingQueue + nodeIdx, 
          m_transparentRenderingQueue + nodeIdx + 1,
          m_transparentNodesCount - nodeIdx);
   m_transparentNodesCount--;
}

///////////////////////////////////////////////////////////////////////////////

void BasicSceneManager::refreshVisibleLights(int lightLimit)
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
