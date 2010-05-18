#ifndef _CLASSES_REGISTRY_H
#error "This file can only be included from ClassesRegistry.h"
#else

#include <stdexcept>
#include "core\TypeID.h"
#include "core\ClassTemplate.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

template < typename ClassType >
ClassTemplate& ClassesRegistry::defineClass()
{
   TypeID< ClassType > classType;
   ClassHandlesMap::iterator it = m_classHandlesMap.find( classType.hash() );
   if (it != m_classHandlesMap.end())
   {
      // verify that the class's name matches the type name of the defined class .
      // If it doesn't it means that we have a hash- value clash and should
      // change either the hashing function, or the class name.
      ClassTemplate& temp = *m_classes[ it->second ];
      ASSERT( temp.getName() == classType.name(), "Class names clash - consider changing the class name or the hashing function!!!" );

      return temp;
   }
   else
   {
      int handle = (int)m_classes.size();
      m_classes.push_back( new ClassTemplate( classType.hash(), classType.name(), new SolidCreator< ClassType >() ) );
      m_classHandlesMap.insert( std::make_pair( classType.hash(), handle ) );
      m_classNamesMap.insert( std::make_pair( classType.name(), handle ) );

      return *m_classes.back();
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename ClassType >
ClassTemplate& ClassesRegistry::getClassByType()
{
   TypeID< ClassType > classType;
   return getClassByHandle( classType.hash() );
}

///////////////////////////////////////////////////////////////////////////////

template< typename ClassType >
void ClassesRegistry::getClassesMatchingType( std::vector< Class >& outClasses )
{
   TypeID< ClassType > classType;
   Class refClass = getClassByHandle( classType.hash() );

   unsigned int count = m_classes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      Class testedClass( *m_classes[i] );
      if ( refClass.isA( testedClass ) )
      {
         outClasses.push_back( testedClass );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _CLASSES_REGISTRY_H
