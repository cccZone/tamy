#include "core-Renderer\FrustumCuller.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core\CollisionTests.h"
#include "core\Frustum.h"
#include "core\BoundingSphere.h"


///////////////////////////////////////////////////////////////////////////////

void FrustumCuller::setup(Camera& camera,
                          Array<AbstractGraphicalNodeP>& renderingQueue)
{
   m_frustum = camera.getFrustrum();
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
