#include "core\File.h"
#include "core\Assert.h"
#include "core\Filesystem.h"
#include "core\FilePath.h"
#include <io.h>


///////////////////////////////////////////////////////////////////////////////

File::File( const Filesystem& hostFS, const FilePath& name, const std::ios_base::open_mode openMode )
   : m_hostFS( hostFS )
   , m_name( name )
   , m_openMode( openMode )
{
   std::string openModeStr;

   if ( ( ( openMode & std::ios_base::in ) == std::ios_base::in ) 
     && ( ( openMode & std::ios_base::out ) == std::ios_base::out ) )
   {
      if ( ( openMode & std::ios_base::trunc ) == std::ios_base::trunc )
      {
         openModeStr += "w+";
      }
      else
      {
         openModeStr += "a+";
      }
   }
   else if ( ( openMode & std::ios_base::in ) == std::ios_base::in )
   {
      openModeStr += "r";
   }
   else if ( ( openMode & std::ios_base::out ) == std::ios_base::out )
   {
      openModeStr += "w";
   }

   if ( ( openMode & std::ios_base::binary ) == std::ios_base::binary )
   {
      openModeStr += "b";
   }

   std::string absoluteName = m_name.toAbsolutePath( m_hostFS );

   fopen_s( &m_file, absoluteName.c_str(), openModeStr.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

File::~File()
{
   if ( m_file )
   {
      fclose( m_file );
   }
   m_file = NULL;

   // notify the filesystem that the file edition has finished
   if ( ( m_openMode & std::ios_base::out ) == std::ios_base::out )
   {
      m_hostFS.onFileEditionCompleted( m_name );
   }
}

///////////////////////////////////////////////////////////////////////////////

void File::seek(DWORD offset, std::ios_base::seekdir dir)
{
   ASSERT_MSG( m_file, "File not opened" );

   int whence;
   switch(dir)
   {
   case std::ios_base::beg: 
      {
         whence = SEEK_SET; 
         break;
      }

   case std::ios_base::end: 
      {
         whence = SEEK_END; 
         break;
      }

   case std::ios_base::cur: 
   default:
      {
         whence = SEEK_CUR; 
         break;
      }
   }
   fseek(m_file, offset, whence);
}

///////////////////////////////////////////////////////////////////////////////

std::size_t File::tell() const
{
   ASSERT_MSG( m_file, "File not opened" );
   return ftell(m_file);
}

///////////////////////////////////////////////////////////////////////////////

std::size_t File::read(byte* buffer, std::size_t size)
{
   ASSERT_MSG( m_file, "File not opened" );
   std::size_t bytesRead = fread(buffer, 1, size, m_file);
   return bytesRead;
}

///////////////////////////////////////////////////////////////////////////////

std::size_t File::write(byte* buffer, std::size_t size)
{
   ASSERT_MSG( m_file, "File not opened" );
   std::size_t bytesWritten = fwrite(buffer, 1, size, m_file);
   return bytesWritten;
}

///////////////////////////////////////////////////////////////////////////////

void File::readString(char* outStrData, std::size_t size)
{
   ASSERT_MSG( m_file, "File not opened" );
   fgets(outStrData, size, m_file);
}

///////////////////////////////////////////////////////////////////////////////

void File::writeString(const char* strData)
{
   ASSERT_MSG( m_file, "File not opened" );

   if ( fputs(strData, m_file) == EOF )
   {
      ASSERT_MSG( false, "Can't write a string to a file" );
   }
}

///////////////////////////////////////////////////////////////////////////////

void File::flush()
{
   ASSERT_MSG( m_file, "File not opened" );
   fflush(m_file);
}

///////////////////////////////////////////////////////////////////////////////

bool File::eof() const
{
   ASSERT_MSG( m_file, "File not opened" );
   return feof(m_file) != 0;
}

///////////////////////////////////////////////////////////////////////////////

std::size_t File::size() const
{
   ASSERT_MSG( m_file, "File not opened" );
   return _filelength(m_file->_file); 
}

///////////////////////////////////////////////////////////////////////////////

void File::setSize(std::size_t newSize)
{
   ASSERT_MSG( m_file, "File not opened" );
   if ( _chsize(m_file->_file, newSize) != 0 )
   {
      ASSERT_MSG( false, "Couldn't resize a file" );
   }
}

///////////////////////////////////////////////////////////////////////////////
