#pragma once

#include <string>
#include "core\NodeAction.h"


///////////////////////////////////////////////////////////////////////////////

class NodeActionMock : public NodeAction
{
private:
   int m_executionsCount;

public:
   NodeActionMock()
      : m_executionsCount(0)
   {
   }

   void performNodeAction()
   {
      m_executionsCount++;
   }

   int getExecutionsCount()
   {
      int result = m_executionsCount;
      m_executionsCount = 0;
      return result;
   }
};

///////////////////////////////////////////////////////////////////////////////
