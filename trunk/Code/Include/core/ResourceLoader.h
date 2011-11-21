/// @file   core/ResourceLoader.h
/// @brief  a resource loader interface
#pragma once

#include <string>
#include "core/FilePath.h"


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
protected:
   FilePath          m_loadedFileName;

public:
   virtual ~ResourceLoader() {}

   /**
    * Sets the name of the loaded file.
    *
    * @param fileName
    */
   void setLoadedFileName( const FilePath& fileName ) { m_loadedFileName = fileName; }

   /**
    * Loads a resource.
    *
    * @param rm         host resources manager
    *
    * @param resource   return the main loaded resource
    */
   virtual Resource* load( ResourcesManager& rm, IProgressObserver& observer ) = 0;

   /**
    * Returns the extension of the resource this loader will create.
    */
   virtual std::string getOutputResourceExtension() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

template< typename RESOURCE_TYPE >
class TResourceImporter : public ResourceLoader
{
public:
   virtual ~TResourceImporter() {}

   // -------------------------------------------------------------------------
   // ResourceLoader implementation
   // -------------------------------------------------------------------------
   std::string getOutputResourceExtension() const { return RESOURCE_TYPE::getExtension(); }
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
   Resource* load( ResourcesManager& rm, IProgressObserver& observer );
   std::string getOutputResourceExtension() const { return ""; }
};

///////////////////////////////////////////////////////////////////////////////
