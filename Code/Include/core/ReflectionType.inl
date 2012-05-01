#ifndef _REFLECTION_TYPE_H
#error "This file can only be included from ReflectionType.h"
#else

#include "core/ReflectionTypesRegistry.h"
#include "core/ReflectionTypeComponent.h"
#include "core/ReflectionSaver.h"
#include "core/ReflectionLoader.h"
#include "core/Array.h"
#include "core/OutArrayStream.h"
#include "core/StringUtils.h"


///////////////////////////////////////////////////////////////////////////////

uint ReflectionType::generateId( const std::string& typeName )
{
   return StringUtils::calculateHash( typeName );
}

///////////////////////////////////////////////////////////////////////////////

bool ReflectionType::isExactlyA( const ReflectionType& referenceType ) const
{
   return m_id == referenceType.m_id;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
T* SerializableReflectionType::instantiate() const
{
   if ( m_instantiator )
   {
      return reinterpret_cast< T* >( m_instantiator->instantiate() );
   }
   else
   {
      return reinterpret_cast< T* >( NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool SerializableReflectionType::isAbstract() const
{
   return m_instantiator != NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void SerializableReflectionType::save( const T& object, const ReflectionSaver& dependenciesMapper, OutStream& stream ) const
{
   // save the type info - so that during deserialization we can 
   // instantiate a proper type instance and have it deserialize
   // its data
   stream << m_id;

   // serialize the unique id assigned to the object
   stream << object.m_uniqueId;

   // create a list of classes in the inheritance hierarchy and start serializing them one by one
   std::list< const SerializableReflectionType* > reflectionTypesList;
   mapTypesHierarchy( reflectionTypesList );

   // serialize the hierarchy of types
   // first - save the number of sub types
   stream << reflectionTypesList.size();

   // next - the types data itself
   Array< byte > tempSerializationDataBuf;
   while( !reflectionTypesList.empty() )
   {
      const SerializableReflectionType* nextType = reflectionTypesList.front();
      reflectionTypesList.pop_front();
      // once again serialize the type id, so that we can look it up during
      // deserialization and deserialize the proper data members
      stream << nextType->m_id;

      // use a temporary data buffer - we want to learn how much space 
      // this data chunk will occupy, and serialize that
      // info so that if this class gets removed from the inheritance
      // hierarchy, we can still deserialize the object properly
      OutArrayStream tempDataStream( tempSerializationDataBuf );

      nextType->saveMemberFields( object, dependenciesMapper, tempDataStream );

      // ok - serialize the chunk size
      stream << tempSerializationDataBuf.size();

      // serialize the chunk itself
      stream.save( (byte*)tempSerializationDataBuf, tempSerializationDataBuf.size() );

      // clear the serialization buffer
      tempSerializationDataBuf.clear();
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T* SerializableReflectionType::load( InStream& stream )
{
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();

   // deserialize and instantiate the serialized object
   T* object = NULL;
   {
      uint id = -1;
      stream >> id;

      // find the corresponding type information
      SerializableReflectionType* deserializedType = typesRegistry.findSerializable( id );
      if ( !deserializedType )
      {
         // such a type doesn't exist
         ASSERT_MSG( !deserializedType, "This type is not registered in the specified ReflectionTypesRegistry" );
         return NULL;
      }

      // instantiate object of the deserialized type
      object = deserializedType->instantiate< T >();

      // deserialize its unique id
      stream >> object->m_uniqueId;
   }


   // now start deserializing its properties
   Array< byte > tempDataBuf;
   uint hierarchyTypesCount = 0;
   stream >> hierarchyTypesCount;
   for ( uint i = 0; i < hierarchyTypesCount; ++i )
   {
      // get the sub type id
      uint hierarchyTypeId = -1;
      stream >> hierarchyTypeId;

      // get the size of the corresponding data chunk
      uint dataChunkSize = -1;
      stream >> dataChunkSize;

      // find the type responsible
      SerializableReflectionType* deserializedHierarchyType = typesRegistry.findSerializable( hierarchyTypeId );
      if ( !deserializedHierarchyType )
      {
         // this type doesn't exist anymore - skip the corresponding part of the stream

         if ( dataChunkSize > tempDataBuf.size() )
         {
            // grow the temporary data buffer if necessary
            tempDataBuf.resize( dataChunkSize + 1 );
         }

         stream.load( (byte*)tempDataBuf, dataChunkSize );
      }
      else
      {
         deserializedHierarchyType->loadMemberFields( *object, stream );
      }
   }

   return object;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void SerializableReflectionType::saveMemberFields( const T& object, const ReflectionSaver& dependenciesMapper, OutStream& stream ) const
{
   // serialize the member
   unsigned int membersCount = m_memberFields.size();
   stream << membersCount;

   Array< byte > tempSerializationDataBuf;
   OutArrayStream tempDataStream( tempSerializationDataBuf );

   for ( uint i = 0; i < membersCount; ++i )
   {
      ReflectionTypeComponent* member = m_memberFields[i];

      // save the member id
      stream << member->m_id;

      // save the member data
      member->save( &object, dependenciesMapper, tempDataStream );

      // write the size of the serialized data, and the data itself
      stream << tempSerializationDataBuf.size();
      stream.save( (byte*)tempSerializationDataBuf, tempSerializationDataBuf.size() );

      // clear the temporary data buffer
      tempSerializationDataBuf.clear();
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void SerializableReflectionType::loadMemberFields( T& object, InStream& stream ) const
{
   uint serializedMembersCount = 0;
   stream >> serializedMembersCount;

   uint memberId, dataSize;
   Array< byte > tempDataBuf;

   for ( uint i = 0; i < serializedMembersCount; ++i )
   {
      // read the member id and the number of bytes it occupies
      stream >> memberId;
      stream >> dataSize;

      // find a field with the matching member id
      ReflectionTypeComponent* member = findMemberField( memberId );
      if ( !member )
      {
         if ( dataSize > tempDataBuf.size() )
         {
            // grow the temporary data buffer if necessary
            tempDataBuf.resize( dataSize + 1 );
         }

         stream.load( (byte*)tempDataBuf, dataSize );
      }
      else
      {
         // deserialize the member data
         member->load( &object, stream );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void SerializableReflectionType::mapDependencies( const T& object, ReflectionSaver& dependenciesCollector ) const
{
   // create a list of classes in the inheritance hierarchy and start mapping their dependencies
   std::list< const SerializableReflectionType* > reflectionTypesList;
   mapTypesHierarchy( reflectionTypesList );

   // map the dependencies on particular subtypes
   Array< byte > tempSerializationDataBuf;
   while( !reflectionTypesList.empty() )
   {
      const SerializableReflectionType* nextType = reflectionTypesList.front();
      reflectionTypesList.pop_front();

      unsigned int membersCount = nextType->m_memberFields.size();
      for ( uint i = 0; i < membersCount; ++i )
      {
         ReflectionTypeComponent* member = nextType->m_memberFields[i];
         member->mapDependencies( &object, dependenciesCollector );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void SerializableReflectionType::restoreDependencies( T& object, const ReflectionLoader& dependenciesMapper ) const
{
   // create a list of classes in the inheritance hierarchy and start restoring their dependencies
   std::list< const SerializableReflectionType* > reflectionTypesList;
   mapTypesHierarchy( reflectionTypesList );

   // restore the dependencies on particular subtypes
   Array< byte > tempSerializationDataBuf;
   while( !reflectionTypesList.empty() )
   {
      const SerializableReflectionType* nextType = reflectionTypesList.front();
      reflectionTypesList.pop_front();

      unsigned int membersCount = nextType->m_memberFields.size();
      for ( uint i = 0; i < membersCount; ++i )
      {
         ReflectionTypeComponent* member = nextType->m_memberFields[i];
         member->restoreDependencies( &object, dependenciesMapper );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename T >
void* TSerializableTypeInstantiator< T >::instantiate() const
{
   return new T();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_TYPE_H

