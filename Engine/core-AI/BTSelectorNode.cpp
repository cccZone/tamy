#include "core-AI\BTSelectorNode.h"
#include "core-AI\BTNodeSelectionStrategy.h"


///////////////////////////////////////////////////////////////////////////////

BTSelectorNode::BTSelectorNode(BTNodeSelectionStrategy* selStrategy)
: m_selStrategy(selStrategy)
{}

///////////////////////////////////////////////////////////////////////////////

BTSelectorNode::~BTSelectorNode()
{
   delete m_selStrategy;
   m_selStrategy = NULL;
}

///////////////////////////////////////////////////////////////////////////////

ExecutionState BTSelectorNode::executeAction()
{
   if (m_nodes.size() == 0) {return BT_COMPLETE;}
   unsigned int nodeIdx = m_selStrategy->getNodeIdx(m_nodes);

   if (nodeIdx == -1) 
   {
      return BT_FAILED;
   }
   else
   {
      return m_nodes.at(nodeIdx)->execute();
   }
}

///////////////////////////////////////////////////////////////////////////////

void BTSelectorNode::reset()
{
}

///////////////////////////////////////////////////////////////////////////////
