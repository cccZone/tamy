#pragma once

#include "core\Node.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core\Array.h"
#include "core\Triangle.h"


///////////////////////////////////////////////////////////////////////////////

class NodeA : public Node
{
private:
   Array<Triangle*> m_geometry;

public:
   NodeA() 
      : Node("nodeA") 
   {}

   ~NodeA()
   {
      for (unsigned int i = 0; i < m_geometry.size(); ++i)
      {
         delete m_geometry[i];
      }
   }

   void addTriangle(Triangle* triangle)
   {
      m_geometry.push_back(triangle);
   }

   const Array<Triangle*>& getBoundingGeometry() const {return m_geometry;}

protected:
   void onAccept(NodeVisitor& visitor)
   {
      REGISTER_NODE_VISITOR(TNodesVisitor<NodeA>);
   }
};

///////////////////////////////////////////////////////////////////////////////
