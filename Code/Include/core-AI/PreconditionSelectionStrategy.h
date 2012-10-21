/// @file   core-AI\PreconditionSelectionStrategy.h
/// @brief  precondition-based node selection strategy
#pragma once

#include "core\MemoryRouter.h"
#include "core-AI\BTNodeSelectionStrategy.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This strategy will sequentially evaluate nodes and select the first
 * one whose precondition evaluates positively.
 */
class PreconditionSelectionStrategy : public BTNodeSelectionStrategy
{
   DECLARE_ALLOCATOR( PreconditionSelectionStrategy, AM_DEFAULT );

public:
   unsigned int getNodeIdx(const std::vector<BTNode*>& nodes);
};

///////////////////////////////////////////////////////////////////////////////
