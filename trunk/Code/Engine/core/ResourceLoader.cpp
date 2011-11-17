#include "core/ResourceLoader.h"
#include "core/ResourcesManager.h"
#include "core/Filesystem.h"
#include "core/Serializer.h"
#include "core/FileSerializer.h"
#include "core/IProgressObserver.h"

///////////////////////////////////////////////////////////////////////////////

Resource* DefaultResourceLoader::load( ResourcesManager& rm, IProgressObserver& observer )
{
   observer.initialize( "Loading engine resource " + m_loadedFileName, 1 );

   std::string extension = Filesystem::extractExtension( m_loadedFileName );
   std::ios_base::openmode fileAccessMode = Resource::getFileAccessMode( extension );

   Filesystem& fs = rm.getFilesystem();
   File* file = fs.open( m_loadedFileName, std::ios_base::in | fileAccessMode );
   Loader loader( new FileSerializer( file ) );
   Resource* res = &loader.load( rm );

   observer.advance();

   return res;
}

///////////////////////////////////////////////////////////////////////////////
