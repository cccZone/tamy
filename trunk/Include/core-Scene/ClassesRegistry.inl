#ifndef _CLASSES_REGISTRY_H
#error "This file can only be included from ClassesRegistry.h"
#else

#include <typeinfo>


///////////////////////////////////////////////////////////////////////////////

template <typename ClassType>
int ClassesRegistry::define()
{
   std::string className = typeid(ClassType).name();
   ClassHandlesMap::iterator it = m_handlesMap.find(className);
   if (it != m_handlesMap.end())
   {
      return it->second;
   }
   else
   {
      int handle = (int)m_creators.size();
      m_creators.push_back(new TClassCreator<ClassType> ());
      m_handlesMap.insert(std::make_pair(className, handle));
      return handle;
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _CLASSES_REGISTRY_H
