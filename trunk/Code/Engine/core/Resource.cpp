#include "core\Resource.h"
#include "core\ResourcesManager.h"
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

BEGIN_OBJECT( Resource, Object )
   PROPERTY( std::string, m_filePath )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Resource::Resource( const std::string& filePath )
: m_filePath( filePath )
, m_host( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void Resource::setFilePath( const std::string& path, ResourcesManager* host )
{
   if ( m_host != host )
   {
      throw std::logic_error( "Only the host manager can move a resource to a different path" );
   }
   m_filePath = path;
}

///////////////////////////////////////////////////////////////////////////////

void Resource::saveResource( ExternalDependenciesSet& outExternalDependencies )
{
   if ( m_host == NULL )
   {
      throw std::runtime_error( "Only managed resources can be saved using this method" );
   }

   m_host->save( m_filePath, outExternalDependencies );
}

///////////////////////////////////////////////////////////////////////////////

void Resource::setResourcesManager( ResourcesManager& mgr )
{
   ASSERT( m_host == NULL, "This resource is already added to a resources manager" );
   m_host = &mgr;

   onResourceLoaded( mgr );
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

Class Resource::findResourceClass( const std::string& extension )
{
   ResourceAccessModes& accessModesMap = getResourceAccesModesMap();
   ResourceAccessModes::const_iterator it = accessModesMap.find( extension );
   if ( it != accessModesMap.end() )
   {
      return Class( it->second.typeName );
   }
   else
   {
      return Class();
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
      throw std::runtime_error( std::string( "Resource with extension ") + extension + " is already registered");
   }
}

///////////////////////////////////////////////////////////////////////////////
