#pragma once

/// @file   Engine\BTNodeSelectionStrategy.h
/// @brief  selector of a node that is to be executed by a BTSelectorNodes

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class BTNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * This strategy tells the BTSelectorNode which node to execute.
 */
class BTNodeSelectionStrategy
{
public:
   virtual ~BTNodeSelectionStrategy() {}

   /**
    * Given a vector of possible nodes to choose from, the method
    * selects one and returns its index from the vector.
    *
    * @param nodes      nodes that can be executed
    * @return           index of a node to execute
    */
   virtual unsigned int getNodeIdx(const std::vector<BTNode*>& nodes) = 0;
};

///////////////////////////////////////////////////////////////////////////////
