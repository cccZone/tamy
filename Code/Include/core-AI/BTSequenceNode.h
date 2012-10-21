/// @file   core-AI\BTSequenceNode.h
/// @brief  Node that executed the behavior it aggregates sequentially
#pragma once

#include "core-AI\BTCompositeNode.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This nodes provides the ability to execute a bunch of other nodes
 * in a sequential order.
 */
class BTSequenceNode : public BTCompositeNode
{
   DECLARE_ALLOCATOR( BTSequenceNode, AM_DEFAULT );

private:
   unsigned int m_currNode;

public:
   BTSequenceNode();

protected:
   ExecutionState executeAction();

   void reset();
};

///////////////////////////////////////////////////////////////////////////////
