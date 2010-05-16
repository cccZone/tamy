#include "core\Resource.h"
#include "core\ResourcesManager.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

namespace 
{
   typedef std::map< std::string, std::ios_base::openmode > ResourceAccessModes;
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
      return it->second;
   }
   else
   {
      return 0;
   }
}

///////////////////////////////////////////////////////////////////////////////

void Resource::registerResource( const std::string& extension, AccessMode accessMode )
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

      accessModesMap.insert( std::make_pair( extension, stlAccessMode ) );
   }
   else
   {
      throw std::runtime_error( std::string( "Resource with extension ") + extension + " is already registered");
   }
}

///////////////////////////////////////////////////////////////////////////////
