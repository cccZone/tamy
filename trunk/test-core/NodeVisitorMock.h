#pragma once

#include "core\NodeVisitor.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

class NodeVisitorMock : public NodeVisitor
{
private:
   int m_visitedNodesCount;

public:
   NodeVisitorMock() : m_visitedNodesCount(0) {}

   void visit(Node& node)
   {
      m_visitedNodesCount++;
   }

   int getVisitedNodesCount() const {return m_visitedNodesCount;}
};

///////////////////////////////////////////////////////////////////////////////

class NodeStub : public Node
{
public:
   NodeStub() : Node("nodeStub") {}

protected:
   void onAccept(NodeVisitor& visitor)
   {
      REGISTER_NODE_VISITOR(NodeVisitorMock);
   }
};

///////////////////////////////////////////////////////////////////////////////
