#include "core\Class.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   ClassesRegistry* g_classesRegistry = NULL;

} // anonymous

///////////////////////////////////////////////////////////////////////////////

ClassesRegistry& getClassesRegistry()
{
   if (g_classesRegistry == NULL)
   {
      g_classesRegistry = new ClassesRegistry();
   }
   return *g_classesRegistry;
}

///////////////////////////////////////////////////////////////////////////////

Class::Class()
: m_handle(-1)
{
}

///////////////////////////////////////////////////////////////////////////////

Class::Class(const std::string& name)
{
   m_name = name;
   m_handle = getClassesRegistry().getHandle(m_name);
}

///////////////////////////////////////////////////////////////////////////////

Class::Class(int handle)
{
   m_handle = handle;
   m_name = getClassesRegistry().getClassName(m_handle);
}

///////////////////////////////////////////////////////////////////////////////

Class::Class(const Class& rhs)
{
   m_name = rhs.m_name;
   m_handle = rhs.m_handle;
   m_parents = rhs.m_parents;
}

///////////////////////////////////////////////////////////////////////////////

bool Class::isA(const Class& rhs) const
{
   std::vector<Class> classHierarchy;
   classHierarchy.push_back(rhs);

   Class classType;
   while(classHierarchy.empty() == false)
   {
      classType = classHierarchy.back();
      classHierarchy.pop_back();

      if (classType == *this)
      {
         // we found it - a parent of rhs's matches the type of this class
         return true;
      }

      // .. so far nothing - let's dig deeper
      classType.getParents(classHierarchy);
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Class::isExactlyA(const Class& rhs) const
{
   return m_handle == rhs.m_handle;
}

///////////////////////////////////////////////////////////////////////////////

bool Class::operator==(const Class& rhs) const
{
   return m_handle == rhs.m_handle;
}

///////////////////////////////////////////////////////////////////////////////

bool Class::operator!=(const Class& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool Class::operator<(const Class& rhs) const
{
   return m_handle < rhs.m_handle;
}

///////////////////////////////////////////////////////////////////////////////

void Class::getParents(std::vector<Class>& outParentHandles) const
{
   unsigned int count = m_parents.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      outParentHandles.push_back(Class(m_parents[i]));
   }
}

///////////////////////////////////////////////////////////////////////////////

void Class::addParent(const std::string& classType)
{
   m_parents.push_back(classType);
}

///////////////////////////////////////////////////////////////////////////////
