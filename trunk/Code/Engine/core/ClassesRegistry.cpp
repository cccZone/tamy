#include "core\ClassesRegistry.h"
#include "core\Class.h"
#include <stdexcept>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

ClassesRegistry::ClassesRegistry()
{
   // define external classes we intend to use
   defineClass< bool >();
   defineClass< int >();
   defineClass< unsigned int >();
   defineClass< char >();
   defineClass< unsigned char >();
   defineClass< short >();
   defineClass< unsigned short >();
   defineClass< long >();
   defineClass< unsigned long >();
   defineClass< float >();
   defineClass< double >();
   defineClass< std::string >();
   defineClass< D3DXVECTOR3 >();
   defineClass< D3DXMATRIX >();
   defineClass< D3DXQUATERNION >();
}

///////////////////////////////////////////////////////////////////////////////

ClassesRegistry::~ClassesRegistry()
{
   unsigned int count = m_classes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_classes[ i ];
   }
   m_classes.clear();
   m_classNamesMap.clear();
   m_classHandlesMap.clear();
}

///////////////////////////////////////////////////////////////////////////////

ClassTemplate& ClassesRegistry::getClassByName( const std::string& className )
{
   ClassNameMap::iterator it = m_classNamesMap.find( className );
   if (it != m_classNamesMap.end())
   {
      return *m_classes[ it->second ];
   }
   else
   {
      throw std::out_of_range(className + " hasn't been registered with the reflection mechanism");
   }
}

///////////////////////////////////////////////////////////////////////////////

ClassTemplate& ClassesRegistry::getClassByHandle( unsigned int handle )
{
   ClassHandlesMap::iterator it = m_classHandlesMap.find( handle );
   if (it != m_classHandlesMap.end())
   {
      return *m_classes[ it->second ];
   }
   else
   {
      throw std::out_of_range("No class is assigned to this handle");
   }
}

///////////////////////////////////////////////////////////////////////////////

void ClassesRegistry::getClassesMatchingType( Class refClass, std::vector< Class >& outClasses, bool includeAbstract )
{
   unsigned int count = m_classes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      Class testedClass( *m_classes[i] );
      if ( refClass.isA( testedClass ) && ( includeAbstract || !testedClass.isAbstract() ) )
      {
         outClasses.push_back( testedClass );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
