#include "core-Renderer\FrustumCuller.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core\CollisionTests.h"
#include "core\Frustum.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

void FrustumCuller::setup(Frustum& frustum,
                          Array<AbstractGraphicalNodeP>& renderingQueue)
{
   m_frustum = frustum;
   m_renderingQueue = &renderingQueue;
}

///////////////////////////////////////////////////////////////////////////////

void FrustumCuller::operator()(AbstractGraphicalNode& node)
{
   if (testCollision(m_frustum, node.getBoundingSphere()) == true)
   {
      m_renderingQueue->push_back(&node);
   }
}

///////////////////////////////////////////////////////////////////////////////
