#pragma once

#include "NodeVisitor.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class is used during the rendering process to extract
 * the nodes that can be rendered from the scene.
 *
 * After running its instance as a visitor of the nodes collection,
 * you can access the gathered graphical nodes using 
 * the "() operator" accessor.
 */
template<typename NodeType>
class GraphicalNodesAggregator : public NodeVisitor
{
private:
   std::list<NodeType*> m_nodes;

public:
   void visit(NodeType& node) {m_nodes.push_back(&node);}

   const std::list<NodeType*>& operator()() const {return m_nodes;}
};

///////////////////////////////////////////////////////////////////////////////
