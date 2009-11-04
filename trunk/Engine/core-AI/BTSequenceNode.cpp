#include "core-AI\BTSequenceNode.h"


///////////////////////////////////////////////////////////////////////////////

BTSequenceNode::BTSequenceNode() 
: m_currNode(0) 
{}

///////////////////////////////////////////////////////////////////////////////

ExecutionState BTSequenceNode::executeAction()
{
   if (m_nodes.size() == 0) {return BT_COMPLETE;}

   ExecutionState status = m_nodes.at(m_currNode)->execute();

   if (status == BT_COMPLETE)
   {
      ++m_currNode;
      if (m_currNode < m_nodes.size()) 
      {
         status = BT_IN_PROGRESS;
      }
      else
      {
         m_currNode = 0;
      }
   }

   return status;
}

///////////////////////////////////////////////////////////////////////////////

void BTSequenceNode::reset()
{
   m_currNode = 0;
}

///////////////////////////////////////////////////////////////////////////////
