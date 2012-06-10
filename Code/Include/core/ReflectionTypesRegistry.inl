#ifndef _REFLECTION_TYPES_REGISTRY_H
#error "This file can only be included from ReflectionTypesRegistry.h"
#else

#include "core/ReflectionType.h"
#include "core/ReflectionTypeID.h"
#include "core/ReflectionEnum.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

ReflectionTypesRegistry& ReflectionTypesRegistry::getInstance() 
{ 
   if ( !s_theInstance )
   {
      s_theInstance = new ReflectionTypesRegistry();
   }
   return *s_theInstance; 
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
bool ReflectionTypesRegistry::addSerializableType( const std::string& typeName, SerializableTypeInstantiator* instantiator, const std::string& patchedTypeName )
{
   SerializableReflectionType* type = new SerializableReflectionType( typeName, patchedTypeName );

   // register the type itself
   SerializableTypesMap::const_iterator it = m_serializableTypesMap.find( type->m_id );
   if ( it != m_serializableTypesMap.end() )
   {
      delete type;
      ASSERT_MSG( false, "Trying to override a type definition" );
      return false;
   }
   else
   {
      // create an instantiator for the type, but only for non-abstract types
      type->setupInstantiator( instantiator );

      // setup the type properties
      T::setupReflectionType( *type );

      // add the type to the types map
      m_serializableTypesMap.insert( std::make_pair( type->m_id, type ) );
      m_allTypes.push_back( type );

      // now check if the type is a 'patch type' and if so, add another mapping to it
      if ( type->m_patchedId < (unsigned int)-1 )
      {
         SerializableTypesMap::const_iterator patchTypeIt = m_serializableTypesMap.find( type->m_patchedId );
         if ( patchTypeIt != m_serializableTypesMap.end() )
         {
            ASSERT_MSG( false, "The patched type is already registered - two types can't patch the same type" );
         }
         else
         {
            m_serializableTypesMap.insert( std::make_pair( type->m_patchedId, type ) );
         }
      }

      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
bool ReflectionTypesRegistry::addExternalType( const std::string& typeName )
{
   ReflectionType* type = new ReflectionType( typeName );

   // register the type itself
   BaseTypesMap::const_iterator it = m_externalTypesMap.find( type->m_id );
   if ( it != m_externalTypesMap.end() )
   {
      delete type;
      ASSERT_MSG( false, "Trying to override a type definition" );
      return false;
   }
   else
   {
      // add the type to the types map
      m_externalTypesMap.insert( std::make_pair( type->m_id, type ) );
      m_allTypes.push_back( type );

      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
bool ReflectionTypesRegistry::addEnumType( const std::string& typeName )
{
   TReflectionEnum< T >* type = new TReflectionEnum< T >( typeName );

   // register the type itself
   BaseTypesMap::const_iterator it = m_externalTypesMap.find( type->m_id );
   if ( it != m_externalTypesMap.end() )
   {
      delete type;
      ASSERT_MSG( false, "Trying to override a type definition" );
      return false;
   }
   else
   {
      // setup the type
      type->registerEnumerators();

      // add the type to the types map
      m_externalTypesMap.insert( std::make_pair( type->m_id, type ) );
      m_allTypes.push_back( type );

      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
ReflectionType* ReflectionTypesRegistry::find() const
{
   ReflectionTypeID< T > typeId;
   return find( typeId.m_id );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ReflectionTypesRegistry::getMatchingTypes( std::vector< const ReflectionType* >& outTypes ) const
{
   ReflectionTypeID< T > typeId;
   const ReflectionType* baseType = find( typeId.m_id );
   
   if ( baseType == NULL )
   {
      return;
   }

   uint count = m_allTypes.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_allTypes[i]->isA( *baseType ) )
      {
         outTypes.push_back( m_allTypes[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ReflectionTypesRegistry::getMatchingSerializableTypes( std::vector< const SerializableReflectionType* >& outTypes, bool includeAbstractTypes ) const
{
   ReflectionTypeID< T > typeId;
   getMatchingSerializableTypes( typeId.m_id, outTypes, includeAbstractTypes );
}

///////////////////////////////////////////////////////////////////////////////

ReflectionType& ReflectionTypesRegistry::getGenericEnumType() const
{
   return *m_genericEnumType;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_TYPES_REGISTRY_H
