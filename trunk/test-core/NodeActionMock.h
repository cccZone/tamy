#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class NodeActionMock
{
private:
   int m_executionsCount;

public:
   NodeActionMock()
      : m_executionsCount(0)
   {
   }

   void execute()
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
