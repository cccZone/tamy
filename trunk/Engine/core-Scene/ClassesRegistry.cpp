#include "core-Scene\ClassesRegistry.h"
#include <stdexcept>


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

void* ClassesRegistry::create(int classHandle)
{
   return m_creators.at(classHandle)->create();
}

///////////////////////////////////////////////////////////////////////////////
