#include "core-AI\BTCompositeNode.h"


///////////////////////////////////////////////////////////////////////////////

BTCompositeNode::~BTCompositeNode()
{
   unsigned int count = m_nodes.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_nodes[i];
   }
   m_nodes.clear();
}

///////////////////////////////////////////////////////////////////////////////

void BTCompositeNode::add(BTNode* node)
{
   m_nodes.push_back(node);
}

///////////////////////////////////////////////////////////////////////////////