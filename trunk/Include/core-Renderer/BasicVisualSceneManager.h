#pragma once

#include "core-Renderer\VisualSceneManager.h"
#include "core-Renderer\BatchingStrategy.h"
#include "core\DistanceComparator.h"
#include <set>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * This scene manager doesn't do much - it assigns all the nodes
 * to its root node, so there's not much hierarchy here.
 */
class BasicVisualSceneManager : public VisualSceneManager
{
private:
   std::list<Light*> m_allLights;

   typedef std::multiset<AbstractGraphicalNode*, BatchComparator> GraphicalNodesSet;
   GraphicalNodesSet m_regularGraphicalNodes;
   AbstractGraphicalNodeP* m_regularRenderingQueue;
   DWORD m_regularRenderingQueueSize;

   AbstractGraphicalNodeP* m_transparentRenderingQueue;
   DWORD m_transparentRenderingQueueSize;
   DWORD m_transparentNodesCount;

   DistanceComparator<AbstractGraphicalNode> m_distanceComparator;

   D3DXVECTOR3 m_cachedCameraPos;
   std::list<Light*> m_currentlyVisibleLights;

public:
   BasicVisualSceneManager();
   ~BasicVisualSceneManager();

   const std::list<Light*>& getLights(int lightLimit);

   AbstractGraphicalNodeP* getRegularGraphicalNodes(DWORD& arraySize);

   AbstractGraphicalNodeP* getTransparentGraphicalNodes(DWORD& arraySize);

protected:
   void add(Light& light);

   void remove(Light& light);

   void addRegularNode(AbstractGraphicalNode& node);
   void removeRegularNode(AbstractGraphicalNode& node);

   void addTransparentNode(AbstractGraphicalNode& node);
   void removeTransparentNode(AbstractGraphicalNode& node);

private:
   void refreshVisibleLights(int lightLimit);
};

///////////////////////////////////////////////////////////////////////////////
