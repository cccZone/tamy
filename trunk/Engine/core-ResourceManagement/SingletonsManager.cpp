#include "core-ResourceManagement\SingletonsManager.h"


///////////////////////////////////////////////////////////////////////////////

SingletonsManager::~SingletonsManager()
{
   unsigned int count = m_shared.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_shared[i];
   }
   m_shared.clear();
}

///////////////////////////////////////////////////////////////////////////////
