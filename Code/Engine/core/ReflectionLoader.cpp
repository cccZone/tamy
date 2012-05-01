#include "core/ReflectionLoader.h"
#include "core/InStream.h"
#include "core/InArrayStream.h"
#include "core/ReflectionType.h"
#include "core/ReflectionObject.h"
#include "core/ReflectionObjectsTracker.h"


///////////////////////////////////////////////////////////////////////////////

ReflectionLoader::ReflectionLoader( InStream& stream ) 
   : m_inputStream( stream )
   , m_instancesTracker( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

ReflectionLoader::ReflectionLoader( InStream& stream, ReflectionObjectsTracker& tracker )
   : m_inputStream( stream )
   , m_instancesTracker( &tracker )
{
}

///////////////////////////////////////////////////////////////////////////////

ReflectionLoader::~ReflectionLoader()
{
   // remove the remaining unread objects
   for ( std::list< ReflectionObject* >::iterator it = m_loadedObjects.begin(); it != m_loadedObjects.end(); ++it )
   {
      delete *it;
   }
   m_loadedObjects.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ReflectionLoader::initialize()
{
   // load the serialized dependencies
   bool result = loadDependencies( m_inputStream );
   if ( !result )
   {
      // no dependencies restored - bail
      return;
   }

   // load the indices of serialized objects
   uint serializedObjectsCount = 0;
   m_inputStream >> serializedObjectsCount;
   if ( serializedObjectsCount == 0 )
   {
      // no serialized objects - something's wrong
      ASSERT_MSG( serializedObjectsCount > 0, "There are no serialized objects indices, even though we managed to load some dependencies" );
      
      // delete the deserialized dependencies
      for ( std::vector< ReflectionObject* >::iterator it = m_dependencies.begin(); it != m_dependencies.end(); ++it )
      {
         delete *it;
      }
      m_dependencies.clear();
      return;
   }

   // deserialize the objects
   for ( uint i = 0; i < serializedObjectsCount; ++i )
   {
      uint serializedObjectIdx = -1;
      m_inputStream >> serializedObjectIdx;
      
      m_loadedObjects.push_back( m_dependencies[serializedObjectIdx] );
   }

   // clean temporary data
   m_dependencies.clear();
}

///////////////////////////////////////////////////////////////////////////////

bool ReflectionLoader::loadDependencies( InStream& stream )
{
   // first - load the dependencies
   uint dependenciesCount = 0;
   stream >> dependenciesCount;
   if ( dependenciesCount == 0 )
   {
      return false;
   }

   for ( uint i = 0; i < dependenciesCount; ++i )
   {
      // first load the dependency id
      ReflectionObject::UniqueId objId;
      stream >> objId;

      uint skipSize = 0;
      stream >> skipSize;

      // if we are using an instances tracker, so see if such an object already
      // exists in the system, maybe there's no need  deserializing it
      ReflectionObject* dependency = NULL;
      if ( m_instancesTracker && ( dependency = m_instancesTracker->findInstance( objId ) ) != NULL )
      {
         // yep - it's there, just take the existing instance and skip that part of the stream
         Array< byte > skipBuf( skipSize ); 
         skipBuf.resize( skipSize );
         stream.load( (byte*)skipBuf, skipSize );
      }
      else
      {
         // this is a new instance - load it
         dependency = SerializableReflectionType::load< ReflectionObject >( stream );

         // and if we're using an instances tracker, add it to it
         if ( m_instancesTracker )
         {
            m_instancesTracker->trackInstance( dependency );
         }
      }
      
      m_dependencies.push_back( dependency );
   }

   // now that we have the dependencies, it's time to map them in the loaded objects
   for ( uint i = 0; i < dependenciesCount; ++i )
   {
      ReflectionObject& object = *m_dependencies[i];
      const SerializableReflectionType& depTypeInfo = object.getVirtualRTTI();
      depTypeInfo.restoreDependencies( object, *this );
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////
