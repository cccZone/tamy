#include "core\InFileStream.h"
#include "core\File.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

InFileStream::InFileStream( File* archive )
   : m_archive( archive )
{
   if ( m_archive == NULL )
   {
      throw std::invalid_argument("NULL pointer instead a File instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

InFileStream::~InFileStream()
{
   delete m_archive;
   m_archive = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void InFileStream::load( void* val, unsigned int dataSize )
{
   m_archive->read( (byte*)val, dataSize );
}

///////////////////////////////////////////////////////////////////////////////
