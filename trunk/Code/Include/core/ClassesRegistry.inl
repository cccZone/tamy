#ifndef _CLASSES_REGISTRY_H
#error "This file can only be included from ClassesRegistry.h"
#else

#include <stdexcept>
#include <typeinfo>


///////////////////////////////////////////////////////////////////////////////

template <typename ClassType>
int ClassesRegistry::defineAbstract(const std::string& className)
{
   ClassHandlesMap::iterator it = m_handlesMap.find(className);
   if (it != m_handlesMap.end())
   {
      return it->second;
   }
   else
   {
      int handle = (int)m_creators.size();
      m_creators.push_back(new AbstractClassCreator());
      m_handlesMap.insert(std::make_pair(className, handle));

      std::string realTypeName = typeid(ClassType).name();
      m_typesMap.insert(std::make_pair(realTypeName, handle));

      return handle;
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename ClassType>
int ClassesRegistry::defineSolid(const std::string& className)
{
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

      std::string realTypeName = typeid(ClassType).name();
      m_typesMap.insert(std::make_pair(realTypeName, handle));

      return handle;
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename ClassType>
int ClassesRegistry::getHandle()
{
   std::string realTypeName = typeid(ClassType).name();

   ClassHandlesMap::const_iterator it = m_typesMap.find(realTypeName);
   if (it != m_typesMap.end())
   {
      return it->second;
   }
   else
   {
      throw std::out_of_range(realTypeName + " hasn't been registered with the reflection mechanism");
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _CLASSES_REGISTRY_H
