/// @file   Engine\BTNode.h
/// @brief  behavior tree node abstract
#pragma once

#include "core/MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

class BTPrecondition;

///////////////////////////////////////////////////////////////////////////////

/**
 * This enum contains statuses an executed action can return to inform
 * the tree about the execution progress.
 */
enum ExecutionState
{
   BT_IN_PROGRESS,
   BT_COMPLETE,
   BT_FAILED
};

///////////////////////////////////////////////////////////////////////////////

/**
* This class represents an abstract behavior tree node. Each node
* has a precondition that should be fulfilled before an
* action it describes is executed.
*/
class BTNode
{
   DECLARE_ALLOCATOR( BTNode, AM_DEFAULT );

private:
   BTPrecondition* m_precondition;

public:
   virtual ~BTNode();

   /**
    * Sets a new precondition for the node.
    *
    * @param precondition  a condition that needs to be fulfilled
    *                      before this node's action can be executed.
    */
   void setPrecondition(BTPrecondition* precondition);

   /**
    * The method evaluates the precondition.
    *
    * @return              'true' if the precondition was evaluated positively
    */
   bool checkPrecondition();

   /**
    * The method executes an action associated with the node,
    * evaluating the precondition before it does so.
    *
    * @return              status telling the outcome of action
    *                      execution (failure, success, or in-progress)
    */
   ExecutionState execute();

protected:
   BTNode();

   /**
    * Node actions are implemented as template methods. This is 
    * the template method that should contain an action
    * the particular node implementation should execute.
    * 
    * @return              status telling the outcome of action
    *                      execution
    */
   virtual ExecutionState executeAction() = 0;

   /**
    * The method should reset the state of an action, so that
    * it can be executed from scratch the next time the 'execute'
    * method is called.
    */
   virtual void reset() = 0;
};

///////////////////////////////////////////////////////////////////////////////
