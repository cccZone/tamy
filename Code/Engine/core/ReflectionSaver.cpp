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

void ReflectionSaver::addDependency( const ReflectionObject& dependency )
{
   uint count = m_dependencies.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_dependencies[i] == &dependency )
      {
         // this dependency is already known
         return;
      }
   }

   // notify the object that it's about to be serialized
   const_cast< ReflectionObject& >( dependency ).onObjectPreSave();

   // this is a new dependency
   m_dependencies.push_back( &dependency );
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
