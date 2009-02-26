#pragma once

#include "NodeVisitor.h"
#include "TNodesVisitor.h"
#include "GraphicalNode.h"
#include "BatchingStrategy.h"
#include <set>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class is used during the rendering process to extract
 * the nodes that can be rendered from the scene.
 *
 * After running its instance as a visitor of the nodes collection,
 * you can access the gathered graphical nodes using 
 * the "() operator" accessor.
 */
class GraphicalNodesAggregator : public NodeVisitor, public TNodesVisitor<GraphicalNode>
{
private:
   BatchedNodes m_nodes;

public:
   void visit(GraphicalNode& node);

   const BatchedNodes& operator()();
};

///////////////////////////////////////////////////////////////////////////////
