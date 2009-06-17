#pragma once

#include <d3dx9.h>
#include "core\Array.h"
#include "core\DistanceComparator.h"
#include "core-Renderer\BatchingStrategy.h"


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalNode;
struct Frustum;

///////////////////////////////////////////////////////////////////////////////

class GraphicalNodesProcessor
{
private:
   BatchComparator m_materialsComparator;
   DistanceComparator<AbstractGraphicalNode> m_distanceComparator;

public:
   void operator()(const Array<AbstractGraphicalNode*>& input, 
                   const Frustum& frustum,
                   const D3DXVECTOR3& cameraGlobalPos,
                   Array<AbstractGraphicalNode*>& output);
};

///////////////////////////////////////////////////////////////////////////////
