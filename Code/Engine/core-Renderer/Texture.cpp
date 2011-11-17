#include "core-Renderer\Texture.h"
#include "core-Renderer\Renderer.h"
#include "core.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( Texture, Resource, tex, AM_BINARY )
   PROPERTY( std::string, m_texFileName )
   PROPERTY_EDIT( "Usage", TextureUsage, m_usage )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

Texture::Texture( const std::string& fileName )
   : Resource( Filesystem::changeFileExtension( fileName, Texture::getExtension() ) )
   , m_texFileName( fileName )
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
      throw std::invalid_argument( "Texture file doesn't exist" );
   }
   StreamBuffer<byte> buf(*file);

   bufSize = buf.size();
   if ( bufSize == 0 )
   {
      delete file;
      throw std::invalid_argument( "Empty texture image" );
   }

   imgBuffer = new byte[ bufSize ];
   memcpy( imgBuffer, buf.getBuffer(), bufSize );
   delete file;
}

///////////////////////////////////////////////////////////////////////////////

void Texture::onPreRender( Renderer& renderer )
{
   new ( renderer() ) RCCreateTexture( *this, m_width, m_height );
}

///////////////////////////////////////////////////////////////////////////////

void Texture::onPostRender( Renderer& renderer )
{

}

///////////////////////////////////////////////////////////////////////////////

bool Texture::onEquals( const Texture& rhs ) const
{
   return getIndex() == rhs.getIndex();
}

///////////////////////////////////////////////////////////////////////////////

bool Texture::onLess( const Texture& rhs ) const
{
   return getIndex() < rhs.getIndex();
}

///////////////////////////////////////////////////////////////////////////////
