#include "core\FileSerializer.h"
#include "core\File.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

FileSerializer::FileSerializer(File* archive)
: m_archive(archive)
{
   if (m_archive == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a File instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

FileSerializer::~FileSerializer()
{
   delete m_archive;
   m_archive = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void FileSerializer::write(byte* buf, unsigned int bytesCount)
{
   m_archive->write(buf, bytesCount);
}

///////////////////////////////////////////////////////////////////////////////

std::size_t FileSerializer::read(byte* buf, unsigned int bytesCount)
{
   return m_archive->read(buf, bytesCount);
}

///////////////////////////////////////////////////////////////////////////////
