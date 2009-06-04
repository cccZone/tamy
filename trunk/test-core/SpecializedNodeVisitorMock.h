#pragma once

#include "core\NodeVisitor.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

class SpecializedNodeStub;

///////////////////////////////////////////////////////////////////////////////

class SpecializedNodeVisitorMock : public NodeVisitor
{
private:
   int m_visitedNodesCount;

public:
   SpecializedNodeVisitorMock() : m_visitedNodesCount(0) {}

   void visit(SpecializedNodeStub& node)
   {
      m_visitedNodesCount++;
   }

   int getVisitedNodesCount() const {return m_visitedNodesCount;}
};

///////////////////////////////////////////////////////////////////////////////

class SpecializedNodeStub : public Node
{
public:
   SpecializedNodeStub() : Node("specializedNodeStub", false) {}

protected:
   void onAccept(NodeVisitor& visitor)
   {
      REGISTER_NODE_VISITOR(SpecializedNodeVisitorMock);
   }
};

///////////////////////////////////////////////////////////////////////////////
