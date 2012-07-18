#include "core-Renderer\Texture.h"
#include "core-Renderer\Renderer.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( Texture, tex, AM_BINARY );
   PARENT( ShaderTexture );
   PROPERTY( FilePath, m_texFileName );
   PROPERTY_EDIT( "Usage", TextureUsage, m_usage );
END_RESOURCE();

///////////////////////////////////////////////////////////////////////////////

Texture::Texture( const FilePath& resourceName )
   : Resource( resourceName )
   , m_usage( TU_COLOR )
   , m_width( 0 )
   , m_height( 0 )
{
}

///////////////////////////////////////////////////////////////////////////////

Texture::~Texture()
{
}

///////////////////////////////////////////////////////////////////////////////

void Texture::getBuffer( byte*& imgBuffer, unsigned int& bufSize ) const
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   const Filesystem& fs = resMgr.getFilesystem();

   // load the file
   File* file = fs.open( m_texFileName, std::ios_base::in | std::ios_base::binary );
   if (file == NULL)
   {
      ASSERT_MSG( false, "Texture file doesn't exist" );
      return;
   }
   StreamBuffer<byte> buf(*file);

   bufSize = buf.size();
   if ( bufSize == 0 )
   {
      delete file;
      ASSERT_MSG( false, "Empty texture image" );
      return;
   }

   imgBuffer = new byte[ bufSize ];
   memcpy( imgBuffer, buf.getBuffer(), bufSize );
   delete file;
}

///////////////////////////////////////////////////////////////////////////////
