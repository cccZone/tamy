#include "core.h"
#include "core\Resource.h"
#include "core\ResourcesManager.h"
#include "core\ReflectionTypesRegistry.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

namespace 
{
   struct ResourceDesc
   {
      std::string                typeName;
      std::ios_base::openmode    accessMode;

      ResourceDesc( const std::string& _typeName,
                    const std::ios_base::openmode& _accessMode )
         : typeName( _typeName )
         , accessMode( _accessMode )
      {}
   };

   // -------------------------------------------------------------------------

   typedef std::map< std::string, ResourceDesc > ResourceAccessModes;
   ResourceAccessModes* g_accessModesMap = NULL;

   static ResourceAccessModes& getResourceAccesModesMap()
   {
      if ( g_accessModesMap == NULL )
      {
         g_accessModesMap = new ResourceAccessModes();
      }
      return *g_accessModesMap;
   }
}

///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( Resource )
   PARENT( ReflectionObject);
   PROPERTY( std::vector< ResourceObject* >, m_managedObjects )
   PROPERTY( std::vector< int >, m_freeIds )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Resource::Resource( const FilePath& filePath )
: m_filePath( filePath )
, m_host( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

Resource::~Resource()
{
   // remove the resource from the resources manager
   if ( m_host )
   {
      ASSERT_MSG( false, "You're deleting a resource that's managed by a ResourcesManager - ARE YOU SURE you want to proceed?" );
      m_host->free( this );
   }

   // free all managed objects
   unsigned int count = m_managedObjects.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_managedObjects[i];
   }
   m_managedObjects.clear();
   m_freeIds.clear();
}

///////////////////////////////////////////////////////////////////////////////

void Resource::setFilePath( const FilePath& path, ResourcesManager* host )
{
   if ( m_host != host )
   {
      ASSERT_MSG( false, "Only the host manager can move a resource to a different path" );
      return;
   }
   m_filePath = path;
}

///////////////////////////////////////////////////////////////////////////////

void Resource::saveResource()
{
   if ( m_host == NULL )
   {
      ASSERT_MSG( false, "Only managed resources can be saved using this method" );
      return;
   }

   onResourceSave( *m_host );

   m_host->save( m_filePath );
}

///////////////////////////////////////////////////////////////////////////////

void Resource::setResourcesManager( ResourcesManager& mgr )
{
   ASSERT_MSG( m_host == NULL, "This resource is already added to a resources manager" );
   m_host = &mgr;
}

///////////////////////////////////////////////////////////////////////////////

void Resource::resetResourcesManager()
{
   m_host = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool Resource::isResource( const std::string& extension )
{
   ResourceAccessModes& accessModesMap = getResourceAccesModesMap();
   ResourceAccessModes::const_iterator it = accessModesMap.find( extension );

   return it != accessModesMap.end();
}

///////////////////////////////////////////////////////////////////////////////

std::ios_base::openmode Resource::getFileAccessMode( const std::string& extension )
{
   ResourceAccessModes& accessModesMap = getResourceAccesModesMap();
   ResourceAccessModes::const_iterator it = accessModesMap.find( extension );
   if ( it != accessModesMap.end() )
   {
      return it->second.accessMode;
   }
   else
   {
      return 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

const SerializableReflectionType* Resource::findResourceClass( const std::string& extension )
{
   ResourceAccessModes& accessModesMap = getResourceAccesModesMap();
   ResourceAccessModes::const_iterator it = accessModesMap.find( extension );
   if ( it != accessModesMap.end() )
   {
      ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
      return typesRegistry.findSerializable( ReflectionType::generateId( it->second.typeName ) );
   }
   else
   {
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void Resource::registerResource( const std::string& extension, 
                                 AccessMode accessMode, 
                                 const std::string& typeName )
{
   ResourceAccessModes& accessModesMap = getResourceAccesModesMap();
   ResourceAccessModes::const_iterator it = accessModesMap.find( extension );
   if ( it == accessModesMap.end() )
   {
      std::ios_base::openmode stlAccessMode = 0;
      if ( accessMode == AM_BINARY )
      {
         stlAccessMode = std::ios_base::binary;
      }

      accessModesMap.insert( std::make_pair( extension, ResourceDesc( typeName, stlAccessMode ) ) );
   }
   else
   {
      std::string errMsg = std::string( "Resource with extension ") + extension + " is already registered";
      ASSERT_MSG(false, errMsg.c_str() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Resource::addObject( ResourceObject* object )
{
   ASSERT_MSG( object != NULL, "NULL pointer instead an object instance" );
   int id = -1;

   if ( object )
   {
      if ( m_freeIds.size() > 0 )
      {
         id = m_freeIds.back();
         m_freeIds.pop_back();

         ASSERT_MSG( m_managedObjects[ id ] == NULL, "The supposed free id doesn't point to a free object entry." );
         m_managedObjects[ id ] = object;
      }
      else
      {
         id = m_managedObjects.size();
         m_managedObjects.push_back( object );
      }

      object->setHostResource( *this, id );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Resource::removeObject( int objectId )
{
   ASSERT_MSG( objectId < (int)m_managedObjects.size(), "Trying to remove an object that doesn't exist" );
   if ( objectId >= (int)m_managedObjects.size() )
   {
      return;
   }

   ASSERT_MSG( m_managedObjects[ objectId ] != NULL, "Trying to remove an object that's already been removed" );
   if ( (int)m_managedObjects[ objectId ] == NULL )
   {
      return;
   }

   m_freeIds.push_back( objectId );
   if ( m_managedObjects[ objectId ] != NULL )
   {
      m_managedObjects[ objectId ]->resetHostResource();
   }

   delete m_managedObjects[ objectId ];
   m_managedObjects[ objectId ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Resource::removeObjectExternally( int objectId )
{
   ASSERT_MSG( objectId < (int)m_managedObjects.size(), "Trying to remove an object that doesn't exist" );
   if ( objectId >= (int)m_managedObjects.size() )
   {
      return;
   }

   ASSERT_MSG( m_managedObjects[ objectId ] != NULL, "Trying to remove an object that's already been removed" );
   if ( (int)m_managedObjects[ objectId ] == NULL )
   {
      return;
   }

   m_freeIds.push_back( objectId );
   m_managedObjects[ objectId ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

ResourceObject& Resource::getObject( int objectId )
{
   if ( objectId >= (int)m_managedObjects.size() )
   {
      ASSERT_MSG( objectId < (int)m_managedObjects.size(), "Trying to access an object that doesn't exist" );
      return *((ResourceObject*)NULL);
   }

   if ( (int)m_managedObjects[ objectId ] == NULL )
   {
      ASSERT_MSG( m_managedObjects[ objectId ] != NULL, "Trying to access an object that was removed" );
      return *((ResourceObject*)NULL);
   }

   return *( m_managedObjects[ objectId ] );
}

///////////////////////////////////////////////////////////////////////////////

void Resource::finalizeResourceLoading()
{
   ASSERT_MSG( isManaged(), "Trying to access an object that was removed" );

   // inform the resource about the registered components
   onResourceLoaded( *m_host );

   unsigned int count = m_host->getComponentsCount();
   for ( unsigned int i = 0; i < count; ++i )
   {
      onComponentAdded( *m_host->getComponent( i ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
