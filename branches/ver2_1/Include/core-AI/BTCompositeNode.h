#pragma once

/// @file   core-AI\BTCompositeNode.h
/// @brief  a composite of beavior tree nodes

#include "core-AI\BTNode.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * 'A single node doesn't make a tree' a proverb says - that's why we need
 * a composite to build the behavior trees.
 */
class BTCompositeNode : public BTNode
{
protected:
   std::vector<BTNode*> m_nodes;

public:
   virtual ~BTCompositeNode();

   /**
    * The method adds a new node to the behavior tree.
    *
    * @param node    node to add
    */
   void add(BTNode* node);

protected:
   // -------------------------------------------------------------------------
   // Inherited from BTNode
   // -------------------------------------------------------------------------
   virtual ExecutionState executeAction() = 0;

   virtual void reset() = 0;
};

///////////////////////////////////////////////////////////////////////////////
