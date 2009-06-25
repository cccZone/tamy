#include "core-ResourceManagement\ResourceManager.h"
#include "core-ResourceManagement\AbstractResourceFactory.h"


///////////////////////////////////////////////////////////////////////////////

ResourceManager::~ResourceManager()
{
   unsigned int count = m_factories.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_factories[i];
   }
   m_factories.clear();

   count = m_shared.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_shared[i];
   }
   m_shared.clear();
}

///////////////////////////////////////////////////////////////////////////////
