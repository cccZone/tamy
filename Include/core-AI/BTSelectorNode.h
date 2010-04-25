#pragma once

/// @file   core-AI\BTSelectorNode.h
/// @brief  node that selects a node it will execute

#include "core-AI\BTCompositeNode.h"


///////////////////////////////////////////////////////////////////////////////

class BTNodeSelectionStrategy;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class selects a node it will execute from a range of
 * nodes it aggregates.
 */
class BTSelectorNode : public BTCompositeNode
{
private:
   BTNodeSelectionStrategy* m_selStrategy;

public:
   /**
    * Constructor.
    *
    * @param selStrategy   strategy of nodes selection.
    */
   BTSelectorNode(BTNodeSelectionStrategy* selStrategy);

   ~BTSelectorNode();

protected:
   ExecutionState executeAction();

   void reset();
};

///////////////////////////////////////////////////////////////////////////////