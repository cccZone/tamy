#ifndef _REFLECTION_SAVER_H
#error "This file can only be included in ReflectionSaver.h"
#else

#include "core/ReflectionType.h"
#include "core/OutStream.h"
#include "core/Assert.h"



///////////////////////////////////////////////////////////////////////////////

uint ReflectionSaver::findDependency( const ReflectionObject* dependency ) const
{
   uint dependenciesCount = m_dependencies.size();
   for ( uint i = 0; i < dependenciesCount; ++i )
   {
      if ( m_dependencies[i] == dependency )
      {
         // found it
         return i + 1;
      }
   }

   // we didn't map such a dependency - how did it got queried for then?
   ASSERT_MSG( false, "Unmapped dependency queried - check the dependency mapping related code" );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////


#endif // _REFLECTION_SAVER_H
