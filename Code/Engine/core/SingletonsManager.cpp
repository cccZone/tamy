#include "core.h"
#include "core\SingletonsManager.h"
#include <typeinfo>
#include <stdexcept>
#include <string>


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

bool SingletonsManager::add(Manageable* newManageable)
{
   // we can store only the instances of unique classes
   std::string typeName = typeid(*newManageable).name();
   std::string tmpTypeName;

   unsigned int count = m_shared.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      tmpTypeName = typeid(*(m_shared[i])).name();
      if (typeName == tmpTypeName)
      {
         delete newManageable;
         return false;
      }
   }

   // object of this type is not yet registered - we can safely add it to our collections
   m_shared.push_back(newManageable);
   return true;
}

///////////////////////////////////////////////////////////////////////////////
