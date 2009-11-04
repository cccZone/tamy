#include "core-AI\PreconditionSelectionStrategy.h"
#include "core-AI\BTNode.h"


///////////////////////////////////////////////////////////////////////////////

unsigned int PreconditionSelectionStrategy::getNodeIdx(const std::vector<BTNode*>& nodes)
{
   unsigned int count = nodes.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (nodes[i]->checkPrecondition() == true)
      {
         return i;
      }
   }

   return -1;
}

///////////////////////////////////////////////////////////////////////////////
