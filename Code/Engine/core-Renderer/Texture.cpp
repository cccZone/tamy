#include "core-Renderer\Texture.h"
#include "core-Renderer\Renderer.h"
#include "core\File.h"
#include "core\StreamBuffer.h"
#include "core\SingletonsManager.h"
#include "core\FileSystem.h"
#include "core\File.h"
#include "core\ResourcesManager.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_RTTI(Texture)
   PARENT(RendererObject)
END_RTTI

///////////////////////////////////////////////////////////////////////////////

Texture::Texture()
: m_imgBuffer(NULL)
, m_bufSize(0)
{
}

///////////////////////////////////////////////////////////////////////////////

Texture::Texture(Filesystem& fs, const std::string& fileName)
{
   File* file = fs.open(fileName, std::ios_base::in | std::ios_base::binary);
   if (file == NULL)
   {
      throw std::invalid_argument("Texture file doesn't exist");
   }
   StreamBuffer<byte> buf(*file);
  
   m_name = file->getName();
   m_bufSize = buf.size();
   if (m_bufSize == 0)
   {
      delete file;
      throw std::invalid_argument("Empty texture image");
   }

   m_imgBuffer = new byte[ m_bufSize ];
   memcpy( m_imgBuffer, buf.getBuffer(), m_bufSize);
   delete file;
}

///////////////////////////////////////////////////////////////////////////////

Texture::Texture(const std::string& name,
                 byte* imgBuffer,
                 unsigned int bufSize)
: m_name(name)
, m_imgBuffer(imgBuffer)
, m_bufSize(bufSize)
{
   if (m_imgBuffer == NULL)
   {
      throw std::invalid_argument("Uninitialized texture image");
   }

   if (m_bufSize == 0)
   {
      throw std::invalid_argument("Empty texture image");
   }
}

///////////////////////////////////////////////////////////////////////////////

Texture::~Texture()
{
   delete [] m_imgBuffer; m_imgBuffer = NULL;
   m_bufSize = 0;
}

///////////////////////////////////////////////////////////////////////////////

void Texture::onLoaded(ResourcesManager& mgr)
{
   Renderer& renderer = mgr.getInitializers().shared<Renderer>();
   renderer.implement<Texture>(*this);
}

///////////////////////////////////////////////////////////////////////////////

const std::string& Texture::getName() const
{
   return m_name;
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
