/// @file   Engine\BTPrecondition.h
/// @brief  behavior tree node's precondition
#pragma once

#include "core/MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This interface describes an action that a behavior tree node can check
 * as a precondition necessary to execute the actual node action.
 */
class BTPrecondition
{
public:
   virtual ~BTPrecondition() {}

   /**
    * The method checks the precondition.
    *
    * @return     'true' if the precondition is valid
    */
   virtual bool check() = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This stub precondition will always evaluate positively.
 */
class NullBTPrecondition : public BTPrecondition
{
   DECLARE_ALLOCATOR( NullBTPrecondition, AM_DEFAULT );

public:
   bool check() {return true;}
};

///////////////////////////////////////////////////////////////////////////////
