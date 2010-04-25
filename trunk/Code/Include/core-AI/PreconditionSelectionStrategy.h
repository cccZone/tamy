#pragma once

/// @file   core-AI\PreconditionSelectionStrategy.h
/// @brief  precondition-based node selection strategy
//

#include "core-AI\BTNodeSelectionStrategy.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This strategy will sequentially evaluate nodes and select the first
 * one whose precondition evaluates positively.
 */
class PreconditionSelectionStrategy : public BTNodeSelectionStrategy
{
public:
   unsigned int getNodeIdx(const std::vector<BTNode*>& nodes);
};

///////////////////////////////////////////////////////////////////////////////
