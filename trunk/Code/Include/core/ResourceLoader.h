/// @file   core/ResourceLoader.h
/// @brief  a resource loader interface
#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class Resource;
class ResourcesManager;
class IProgressObserver;

///////////////////////////////////////////////////////////////////////////////

/**
 * A resource loader interface.
 */
class ResourceLoader
{
public:
   virtual ~ResourceLoader() {}

   /**
    * Loads a resource.
    *
    * @param fileName   path to the resource file
    * @param rm         host resources manager
    *
    * @param resource   return the main loaded resource
    */
   virtual Resource* load( const std::string& fileName, ResourcesManager& rm, IProgressObserver& observer ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A resource loader for loading engine-supported resources.
 */
class DefaultResourceLoader : public ResourceLoader
{
public:
   // -------------------------------------------------------------------------
   // ResourceLoader implementation
   // -------------------------------------------------------------------------
   Resource* load( const std::string& fileName, ResourcesManager& rm, IProgressObserver& observer );
};

///////////////////////////////////////////////////////////////////////////////
