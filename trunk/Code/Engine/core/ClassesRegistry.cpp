#include "core\ClassesRegistry.h"
#include "core\Class.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

ClassesRegistry::ClassesRegistry()
{
}

///////////////////////////////////////////////////////////////////////////////

ClassesRegistry::~ClassesRegistry()
{
   unsigned int count = m_creators.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_creators[i];
   }
   m_creators.clear();
}

///////////////////////////////////////////////////////////////////////////////

int ClassesRegistry::getHandle(const std::string& className)
{
   ClassHandlesMap::iterator it = m_handlesMap.find(className);
   if (it != m_handlesMap.end())
   {
      return it->second;
   }
   else
   {
      throw std::out_of_range(className + " hasn't been registered with the reflection mechanism");
   }
}

///////////////////////////////////////////////////////////////////////////////

const std::string& ClassesRegistry::getClassName(int handle) const
{
   for (ClassHandlesMap::const_iterator it = m_handlesMap.begin();
        it != m_handlesMap.end(); ++it)
   {
      if (it->second == handle)
      {
         return it->first;
      }
   }
   
   throw std::out_of_range("No class is assigned to this handle");
}

///////////////////////////////////////////////////////////////////////////////

void* ClassesRegistry::create(int classHandle)
{
   return m_creators.at(classHandle)->create();
}

///////////////////////////////////////////////////////////////////////////////
