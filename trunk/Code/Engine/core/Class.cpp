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
: m_template( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

Class::Class( ClassTemplate& temp )
: m_template( &temp )
{
}

///////////////////////////////////////////////////////////////////////////////

Class::Class( const std::string& name )
: m_template( &getClassesRegistry().getClassByName(name) )
{
}

///////////////////////////////////////////////////////////////////////////////

Class::Class( unsigned int handle )
: m_template( &getClassesRegistry().getClassByHandle( handle ) )
{
}

///////////////////////////////////////////////////////////////////////////////

Class::Class(const Class& rhs)
: m_template( rhs.m_template )
{
}

///////////////////////////////////////////////////////////////////////////////

const std::string& Class::getName() const
{
   return m_template->getName();
}

///////////////////////////////////////////////////////////////////////////////

unsigned int Class::getHandle() const
{
   return m_template->getHandle();
}

///////////////////////////////////////////////////////////////////////////////

bool Class::isA( const Class& rhs ) const
{
   std::vector< Class > classHierarchy;
   classHierarchy.push_back(rhs);

   Class classType;
   while( classHierarchy.empty() == false )
   {
      classType = classHierarchy.back();
      classHierarchy.pop_back();

      if ( classType == *this )
      {
         // we found it - a parent of rhs's matches the type of this class
         return true;
      }

      // .. so far nothing - let's dig deeper
      classType.getParents( classHierarchy );
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool Class::isExactlyA(const Class& rhs) const
{
   return *m_template == *rhs.m_template;
}

///////////////////////////////////////////////////////////////////////////////

bool Class::operator==(const Class& rhs) const
{
   return *m_template == *rhs.m_template;
}

///////////////////////////////////////////////////////////////////////////////

bool Class::operator!=(const Class& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

bool Class::operator<(const Class& rhs) const
{
   return *m_template < *rhs.m_template;
}

///////////////////////////////////////////////////////////////////////////////

void Class::getParents(std::vector< Class >& outParentHandles) const
{
   const std::vector< std::string >& parents = m_template->getParents();
   ClassesRegistry& reg = getClassesRegistry();

   unsigned int count = parents.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      outParentHandles.push_back( Class( parents[i] ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
