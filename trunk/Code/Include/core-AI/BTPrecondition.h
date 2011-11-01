#pragma once

/// @file   Engine\BTPrecondition.h
/// @brief  behavior tree node's precondition


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
public:
   bool check() {return true;}
};

///////////////////////////////////////////////////////////////////////////////