#pragma once

#include "core\Node.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"


///////////////////////////////////////////////////////////////////////////////

class NodeA : public Node
{
protected:
   void onAccept(NodeVisitor& visitor)
   {
      REGISTER_NODE_VISITOR(TNodesVisitor<NodeA>);
   }
};

///////////////////////////////////////////////////////////////////////////////
