#ifndef _STRING_ENUM_H
#error "This file can only be included from Enum.h"
#else

#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

template< typename DerivedType, typename EnumType >
typename EnumBase< DerivedType, EnumType >::AssocMap& EnumBase< DerivedType, EnumType >::getMap()
{
   // A static map of associations from strings to enumerators
   static AssocMap assocMap;
   static bool     firstAccess = true;

   // If this is the first time we're accessing the map, then populate it.
   if( firstAccess )
   {
      firstAccess = false;
      DerivedType::registerEnumerators();

      ASSERT( !assocMap.empty() );
   }

   return assocMap;
}

///////////////////////////////////////////////////////////////////////////////

template< typename DerivedType, typename EnumType >
void EnumBase< DerivedType, EnumType >::registerEnumerator( const EnumType enumerator, const std::string &eStr )
{
   const bool registered = getMap().insert( typename AssocMap::value_type( enumerator, eStr ) ).second;
   ASSERT( registered );
}

///////////////////////////////////////////////////////////////////////////////

template< typename DerivedType, typename EnumType >
const std::string& EnumBase< DerivedType, EnumType >::from( const EnumType enumerator )
{
   static const std::string emptyStr;

   // Search for the string in our map.
   const AssocMap& enumsMap = getMap();
   const typename AssocMap::const_iterator it = enumsMap.find( enumerator );

   // If we have it, then return the associated enumerator.
   if( it != enumsMap.end() )
   {
      return it->second;
   }
   else
   {
      return emptyStr;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename DerivedType, typename EnumType >
const bool EnumBase< DerivedType, EnumType >::to( const std::string& str, EnumType& outEnumerator )
{
   // Search for the string in our map.
   const AssocMap& enumsMap = getMap();

   // search for the enumerator in our map
   typename AssocMap::const_iterator it = enumsMap.begin();
   for ( ; it != enumsMap.end(); ++it )
   {
      if ( it->second == str )
      {
         break;
      }
   }

   // if we didn't find it, we can't do this conversion
   if( it == enumsMap.end() )
   {
      return false;
   }

   // Verify that there's no enumerator with the same value assigned
   typename AssocMap::const_iterator anotherIt = it;
   for ( ++anotherIt; anotherIt != enumsMap.end(); ++anotherIt )
   {
      if ( anotherIt->second == str )
      {
         // unfortunately - there is, which means we can't perform an unambiguous conversion
         return false;
      }
   }

   // we found exactly one string which matches the required enumerator
   outEnumerator = it->first;
   return true;
}

///////////////////////////////////////////////////////////////////////////////

template< typename DerivedType, typename EnumType >
int EnumBase< DerivedType, EnumType >::getIndex( int enumeratorValue ) const
{
   // Search for the string in our map.
   const AssocMap& enumsMap = getMap();

   int idx = 0;
   for ( typename AssocMap::const_iterator it = enumsMap.begin(); it != enumsMap.end(); ++it, ++idx )
   {
      if ( it->first == enumeratorValue )
      {
         return idx;
      }
   }

   // if we got here, it means there's no enumerator with the specified value assigned
   return -1;
}

///////////////////////////////////////////////////////////////////////////////

template< typename DerivedType, typename EnumType >
void EnumBase< DerivedType, EnumType >::getEnumerators( std::vector< std::string >& outEnumerators ) const
{
   const AssocMap& enumsMap = getMap();
   for ( AssocMap::const_iterator it = enumsMap.begin(); it != enumsMap.end(); ++it )
   {
      outEnumerators.push_back( it->second );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename DerivedType, typename EnumType >
int EnumBase< DerivedType, EnumType >::getValue( const std::string& enumeratorName ) const
{
   EnumType val;
   if ( TEnum< EnumType >::to( enumeratorName, val ) )
   {
      return val;
   }
   else
   {
      return -1;
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
TypeID< TEnum< T > >::TypeID()
{
   // create the type name
   std::string name = typeid( T ).name();

   // remove 'class', 'struct', 'enum' prefixes
   std::size_t pos = name.find( "class " );
   if ( pos != std::string::npos )
   {
      name = name.substr( pos + 6 );
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
   m_name = name;

   // calculate the hash value
   m_hash = StringUtils::calculateHash( m_name );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const std::string& TypeID< TEnum< T > >::name() const 
{ 
   return m_name; 
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
unsigned int TypeID< TEnum< T > >::hash() const 
{ 
   return m_hash; 
}

///////////////////////////////////////////////////////////////////////////////

#endif // _STRING_ENUM_H
