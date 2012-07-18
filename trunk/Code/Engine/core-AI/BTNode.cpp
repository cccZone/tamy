#include "core-AI\BTNode.h"
#include "core-AI\BTPrecondition.h"
#include "core\Assert.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

BTNode::BTNode() 
: m_precondition(new NullBTPrecondition()) 
{
}

///////////////////////////////////////////////////////////////////////////////

BTNode::~BTNode() 
{
   delete m_precondition;
   m_precondition = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void BTNode::setPrecondition(BTPrecondition* precondition)
{
   if (precondition == NULL)
   {
      ASSERT_MSG( false, "NULL pointer instead a BTPrecondition instance" );
   }

   delete m_precondition;
   m_precondition = precondition;
}

///////////////////////////////////////////////////////////////////////////////

bool BTNode::checkPrecondition()
{
   return m_precondition->check();
}

///////////////////////////////////////////////////////////////////////////////

ExecutionState BTNode::execute()
{
   if (checkPrecondition() == false)
   {
      reset();
      return BT_FAILED;
   }
   else
   {
      return executeAction();
   }
}

///////////////////////////////////////////////////////////////////////////////
