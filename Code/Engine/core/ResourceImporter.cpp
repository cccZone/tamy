#include "core.h"
#include "core/ResourceImporter.h"
#include "core/IProgressObserver.h"


///////////////////////////////////////////////////////////////////////////////

ResourceImporter::ResourceImporter( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer ) 
   : m_loadedFileName( path )
   , m_rm( rm )
   , m_observer( observer ) 
{}

///////////////////////////////////////////////////////////////////////////////

ResourceImporter::~ResourceImporter()
{
   delete m_observer;
   m_observer = NULL;
}

///////////////////////////////////////////////////////////////////////////////
