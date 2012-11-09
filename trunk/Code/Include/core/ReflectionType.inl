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
   return m_instantiator == NULL;
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

      nextType->saveMemberFields( &object, dependenciesMapper, tempDataStream );

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
      ReflectionObject::UniqueId objectUniqueId; 
      stream >> id;
      stream >> objectUniqueId;

      // find the corresponding type information
      SerializableReflectionType* deserializedType = typesRegistry.findSerializable( id );
      if ( deserializedType )
      {
         // instantiate object of the deserialized type
         SerializationFlag::getInstance().flagSerializationInProgress( true );
         object = deserializedType->instantiate< T >();
         SerializationFlag::getInstance().flagSerializationInProgress( false );
         object->m_uniqueId = objectUniqueId;
      }
   }


   // Now start deserializing its properties - whether we managed to deserialize the object or not.
   // we need to be able to read it.
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
         deserializedHierarchyType->loadMemberFields( object, stream );
      }
   }

   return object;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void SerializableReflectionType::saveMemberFields( const T* object, const ReflectionSaver& dependenciesMapper, OutStream& stream ) const
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
      member->save( object, dependenciesMapper, tempDataStream );

      // write the size of the serialized data, and the data itself
      stream << tempSerializationDataBuf.size();
      stream.save( (byte*)tempSerializationDataBuf, tempSerializationDataBuf.size() );

      // clear the temporary data buffer
      tempSerializationDataBuf.clear();
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void SerializableReflectionType::loadMemberFields( T* object, InStream& stream ) const
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
         member->load( object, stream );
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

