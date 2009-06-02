#include "core-Renderer\NullCuller.h"
#include "core-Renderer\AbstractGraphicalNode.h"


///////////////////////////////////////////////////////////////////////////////

void NullCuller::setup(Frustum& frustum,
                       Array<AbstractGraphicalNodeP>& renderingQueue)
{
   m_renderingQueue = &renderingQueue;
}

///////////////////////////////////////////////////////////////////////////////

void NullCuller::operator()(AbstractGraphicalNode& node)
{
   m_renderingQueue->push_back(&node);
}

///////////////////////////////////////////////////////////////////////////////
