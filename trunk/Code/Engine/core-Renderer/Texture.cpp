#include "core-Renderer\Texture.h"
#include "core-Renderer\Renderer.h"
#include "core\StreamBuffer.h"
#include "core\SingletonsManager.h"
#include "core\FileSystem.h"
#include "core\File.h"
#include "core\ResourcesManager.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCE( Texture, Resource, tex, AM_BINARY )
   PROPERTY_EDIT( "imageFile", std::string, m_texFileName )
END_RESOURCE()

///////////////////////////////////////////////////////////////////////////////

Texture::Texture( const std::string& fileName )
: Resource( Filesystem::changeFileExtension( fileName, Texture::getExtension() ) )
, m_texFileName( fileName )
, m_imgBuffer(NULL)
, m_bufSize(0)
{
}

///////////////////////////////////////////////////////////////////////////////

void Texture::loadFromFile( const Filesystem& fs )
{
   // cleanup previous data
   delete [] m_imgBuffer;
   m_imgBuffer = NULL;
   m_bufSize = 0;

   // load the file
   File* file = fs.open( m_texFileName, std::ios_base::in | std::ios_base::binary );
   if (file == NULL)
   {
      throw std::invalid_argument( "Texture file doesn't exist" );
   }
   StreamBuffer<byte> buf(*file);
  
   m_bufSize = buf.size();
   if (m_bufSize == 0)
   {
      delete file;
      throw std::invalid_argument( "Empty texture image" );
   }

   m_imgBuffer = new byte[ m_bufSize ];
   memcpy( m_imgBuffer, buf.getBuffer(), m_bufSize);
   delete file;
}

///////////////////////////////////////////////////////////////////////////////

Texture::~Texture()
{
   delete [] m_imgBuffer; m_imgBuffer = NULL;
   m_bufSize = 0;
}

///////////////////////////////////////////////////////////////////////////////

void Texture::onResourceLoaded( ResourcesManager& mgr )
{
   loadFromFile( mgr.getFilesystem() );
   Renderer& renderer = mgr.getInitializers().shared<Renderer>();
   renderer.implement<Texture>(*this);
}

///////////////////////////////////////////////////////////////////////////////

void* Texture::getPlatformSpecific() const
{
   return impl().getPlatformSpecific();
}

///////////////////////////////////////////////////////////////////////////////

void Texture::getBuffer(unsigned int& outSize, byte** outBuffer)
{
   outSize = m_bufSize;
   *outBuffer = m_imgBuffer;
}

///////////////////////////////////////////////////////////////////////////////

void Texture::releaseData()
{
   delete [] m_imgBuffer; m_imgBuffer = NULL;
   m_bufSize = 0;
}

///////////////////////////////////////////////////////////////////////////////
