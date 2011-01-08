#ifndef _TYPE_ID_H
#error "This file can only be included from TypeID.h"
#else

#include <typeinfo.h>
#include "core\StringUtils.h"
#include <vector>
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T >
TypeID< T >::TypeID()
{
   // create the type name
   std::vector< std::string > nameParts;
   StringUtils::tokenize( typeid( T ).name(), "<>", nameParts );

   m_name = getSingleTypeName( nameParts[ 0 ] );

   unsigned int count = nameParts.size();
   for ( unsigned int i = 1; i < count; ++i )
   {
      m_name += std::string( "< " ) + getSingleTypeName( nameParts[ i ] );
   }

   for ( unsigned int i = 1; i < count; ++i )
   {
      m_name += " >";
   }

   // calculate the hash value
   m_hash = StringUtils::calculateHash( m_name );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
std::string TypeID< T >::getSingleTypeName( const std::string& typeName ) const
{
   std::string name = typeName;

   // remove 'class', 'struct', 'enum' prefixes
   std::size_t pos = name.find( "class " );
   if ( pos != std::string::npos )
   {
      name = name.substr( pos + 6 );
   }
   pos = name.find( "struct " );
   if ( pos != std::string::npos )
   {
      name = name.substr( pos + 7 );
   }
   pos = name.find( "enum " );
   if ( pos != std::string::npos )
   {
      name = name.substr( pos + 5 );
   }

   // remove all preceding namespaces
   while ( ( pos = name.find( "anonymous namespace" ) ) != std::string::npos )
   {
      name = name.substr( pos + 20 );
   }

   return name;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const std::string& TypeID< T >::name() const
{
   return m_name;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
unsigned int TypeID< T >::hash() const
{
   return m_hash;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _TYPE_ID_H
