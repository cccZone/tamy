#include "core/ReflectionSaver.h"
#include "core/OutStream.h"
#include "core/OutArrayStream.h"
#include "core/ReflectionObject.h"

///////////////////////////////////////////////////////////////////////////////

ReflectionSaver::ReflectionSaver( OutStream& stream ) 
   : m_outStream( stream )
{
}

///////////////////////////////////////////////////////////////////////////////

ReflectionSaver::~ReflectionSaver()
{
   flush();
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSaver::save( const ReflectionObject* object )
{
   // Check if the object isn't already stored somewhere in the dependencies map.
   // If it is, there's no point in serializing it again.
   uint count = m_dependencies.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_dependencies[i] == object )
      {
         // it was already serialized - just add its dependency index to the saved objects list
         m_serializedObjectsIndices.push_back( i );
         return;
      }
   }

   // Create a map of all pointers we're about to serialize.
   // In other words - map the dependencies the saved object has

   // ...and the very first dependency is our object itself - memorize its index so that we know
   // which object on the long list of serialized objects was the serialized one
   m_objectsToMap.clear();
   m_serializedObjectsIndices.push_back( m_dependencies.size() );
   addDependency( object );

   // map its dependencies
   while( !m_objectsToMap.empty() )
   {
      const ReflectionObject* objectToMap = m_objectsToMap.back(); m_objectsToMap.pop_back();
      ASSERT( objectToMap != NULL );
      mapDependencies( objectToMap );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSaver::addDependency( const ReflectionObject* dependency )
{
   if ( !dependency )
   {
      return;
   }

   uint count = m_dependencies.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_dependencies[i] == dependency )
      {
         // this dependency is already known
         return;
      }
   }

   // notify the object that it's about to be serialized
   const_cast< ReflectionObject* >( dependency )->onObjectPreSave();

   // this is a new dependency
   m_dependencies.push_back( dependency );
   m_objectsToMap.push_back( dependency );
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSaver::mapDependencies( const ReflectionObject* object )
{
   const SerializableReflectionType& objType = object->getVirtualRTTI();

   // create a list of classes in the inheritance hierarchy and start mapping their dependencies
   std::list< const SerializableReflectionType* > reflectionTypesList;
   objType.mapTypesHierarchy( reflectionTypesList );

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
         member->mapDependencies( object, *this );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionSaver::flush()
{
   // First, we need to serialize the dependencies map.
   {
      // number of stored dependencies
      uint dependenciesCount = m_dependencies.size();
      m_outStream << dependenciesCount;

      // dependencies themselves
      for ( uint i = 0; i < dependenciesCount; ++i )
      {
         // store the unique id of the type so that we can look it up quickly afterwards
         m_outStream << m_dependencies[i]->m_uniqueId;

         // save the data to a temp buffer, 'cause we'll be needing a skip size of the data
         // so that if we manage to find this instance in our records during deserialization ( providing
         // that we keep track of course ), we can just skip reading it
         {
            Array< byte > tmpDataBuf;
            OutArrayStream tmpDataStream( tmpDataBuf );

            const SerializableReflectionType& depTypeInfo = m_dependencies[i]->getVirtualRTTI();
            depTypeInfo.save( *m_dependencies[i], *this, tmpDataStream );

            // store the size of the data
            uint dataSize = tmpDataBuf.size();
            m_outStream << dataSize;

            // and then the data itself
            m_outStream.save( (byte*)tmpDataBuf, dataSize );
         }
      }

      // clear the dependencies list
      m_dependencies.clear();
   }

   // now serialize the indices of the saved objects
   {
      uint savedObjectsCount = m_serializedObjectsIndices.size();
      m_outStream << savedObjectsCount;

      for ( uint i = 0; i < savedObjectsCount; ++i )
      {
         m_outStream << m_serializedObjectsIndices[i];
      }

      // clear the saved objects indices list
      m_serializedObjectsIndices.clear();
   }
}

///////////////////////////////////////////////////////////////////////////////
