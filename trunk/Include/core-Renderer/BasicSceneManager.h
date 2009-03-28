#pragma once

#include "core-Renderer\SceneManager.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\BatchingStrategy.h"
#include <set>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * This scene manager doesn't do much - it assigns all the nodes
 * to its root node, so there's not much hierarchy here.
 */
class BasicSceneManager : public SceneManager,
                          public NodeVisitor, 
                          public TNodesVisitor<Light>,
                          public TNodesVisitor<AbstractGraphicalNode>
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

   void addNode(Node* newNode);

   const std::list<Light*>& getLights(const Node& cameraNode, int lightLimit);

   AbstractGraphicalNodeP* getRegularGraphicalNodes(const Node& cameraNode, 
                                                    DWORD& arraySize);

   AbstractGraphicalNodeP* getTransparentGraphicalNodes(const Node& cameraNode, 
                                                        DWORD& arraySize);

   void visit(Light& light) {m_allLights.push_back(&light);}
   void visit(AbstractGraphicalNode& node);

private:
   void refreshVisibleLights(int lightLimit);
};

///////////////////////////////////////////////////////////////////////////////
