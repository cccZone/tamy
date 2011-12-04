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
class ResourceImporter
{
protected:
   FilePath             m_loadedFileName;
   ResourcesManager&    m_rm;
   IProgressObserver*   m_observer;

public:
   virtual ~ResourceImporter();

   /**
    * Checks if the importer can import the specified resource.
    *
    * @param resourceType
    */
   virtual bool canImport( const Class& resourceType ) const = 0;

protected:
   /**
    * Constructor.
    *
    * @param path
    * @param rm         resources manager
    * @param observer
    */
   ResourceImporter( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer );
};

///////////////////////////////////////////////////////////////////////////////

template< typename ResourceType >
class TResourceImporter : public ResourceImporter
{
public:
   /**
    * Constructor.
    *
    * @param   path
    * @param rm         resources manager
    * @param observer
    */
   TResourceImporter( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer ) : ResourceImporter( path, rm, observer ) {}
   virtual ~TResourceImporter() {}

   /**
    * Imports the resource.
    *
    * @param resource         resource the contents of which we want to import
    */
   virtual void import( ResourceType& resource ) = 0;

   // -------------------------------------------------------------------------
   // ResourceImporter implementation
   // -------------------------------------------------------------------------
   inline bool canImport( const Class& resourceType ) const { return resourceType.isExactlyA( ResourceType::getRTTIClass() ); }

};

///////////////////////////////////////////////////////////////////////////////
