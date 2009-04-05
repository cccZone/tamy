#pragma once

#include "core-Renderer\SceneManager.h"
#include "core-Renderer\BatchingStrategy.h"
#include <set>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * This scene manager doesn't do much - it assigns all the nodes
 * to its root node, so there's not much hierarchy here.
 */
class BasicSceneManager : public SceneManager
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

   DistanceComparator m_distanceComparator;

   D3DXVECTOR3 m_cachedCameraPos;
   std::list<Light*> m_currentlyVisibleLights;

public:
   BasicSceneManager();
   ~BasicSceneManager();

   const std::list<Light*>& getLights(int lightLimit);

   AbstractGraphicalNodeP* getRegularGraphicalNodes(DWORD& arraySize);

   AbstractGraphicalNodeP* getTransparentGraphicalNodes(DWORD& arraySize);

protected:
   void addToHierarchy(Node* node);

   void removeFromHierarchy(Node* node);

   void add(Light& light);

   void remove(Light& light);

   void add(AbstractGraphicalNode& node);

   void remove(AbstractGraphicalNode& node);

private:
   void refreshVisibleLights(int lightLimit);

   void addRegularNode(AbstractGraphicalNode& node);
   void removeRegularNode(AbstractGraphicalNode& node);

   void addTransparentNode(AbstractGraphicalNode& node);
   void removeTransparentNode(AbstractGraphicalNode& node);
};

///////////////////////////////////////////////////////////////////////////////
