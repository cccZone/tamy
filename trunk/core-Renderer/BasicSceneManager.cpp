#include "core-Renderer\BasicSceneManager.h"
#include "core\Node.h"
#include "core-Renderer\Material.h"
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

void BasicSceneManager::addNode(Node* newNode)
{
   // attach the node to the nodfes hierarchy
   getRootNode().addChild(newNode);

   // check its kind to see what buffer shoul it be put in
   newNode->accept(*this);
}

///////////////////////////////////////////////////////////////////////////////

const std::list<Light*>& BasicSceneManager::getLights(const Node& cameraNode, 
                                                      int lightLimit)
{
   bool dirty = (m_currentlyVisibleLights.size() == 0) || 
                (cameraNode.getPosition() != m_cachedCameraPos) ||
                (m_currentlyVisibleLights.size() > lightLimit);

   if (dirty)
   {
      m_cachedCameraPos = cameraNode.getPosition();
      refreshVisibleLights(lightLimit);
   }

   return m_currentlyVisibleLights;
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalNodeP* BasicSceneManager::getRegularGraphicalNodes(const Node& cameraNode, 
                                                                    DWORD& arraySize)
{
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

AbstractGraphicalNodeP* BasicSceneManager::getTransparentGraphicalNodes(const Node& cameraNode, 
                                                                        DWORD& arraySize)
{
   if (m_transparentNodesCount > 0)
   {
      m_distanceComparator.setReferencePoint(cameraNode.getPosition());
      std::sort(m_transparentRenderingQueue, 
                m_transparentRenderingQueue + m_transparentNodesCount,
                m_distanceComparator);
   }

   arraySize = m_transparentNodesCount;
   return m_transparentRenderingQueue;
}

///////////////////////////////////////////////////////////////////////////////

void BasicSceneManager::visit(AbstractGraphicalNode& node) 
{
   if (node.getMaterial().isTransparent())
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
   else
   {
      m_regularGraphicalNodes.insert(&node);  
   }
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
