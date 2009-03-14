#pragma once

#include "NodeVisitor.h"
#include "TNodesVisitor.h"
#include "AbstractGraphicalNode.h"
#include "BatchingStrategy.h"
#include <set>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class is used during the rendering process to extract
 * the nodes that can be rendered from the scene.
 *
 * After running its instance as a visitor of the nodes collection,
 * you can access the gathered graphical nodes using 
 * the "() operator" accessor.
 */
class GraphicalNodesAggregator : public NodeVisitor, 
                                 public TNodesVisitor<AbstractGraphicalNode>
{
private:
   BatchComparator m_comparator;
   BatchedNodes m_nodes;

public:
   GraphicalNodesAggregator(const D3DXVECTOR3& cameraPos);

   void visit(AbstractGraphicalNode& node);

   const BatchedNodes& operator()();
};

///////////////////////////////////////////////////////////////////////////////
