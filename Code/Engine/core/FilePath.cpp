#include "core/FilePath.h"
#include "core/InStream.h"
#include "core/OutStream.h"
#include "core/Filesystem.h"


///////////////////////////////////////////////////////////////////////////////

FilePath::FilePath()
{
}

///////////////////////////////////////////////////////////////////////////////

FilePath::~FilePath()
{
}

///////////////////////////////////////////////////////////////////////////////

FilePath::FilePath( const std::string& path )
{
   Filesystem::normalize( path, m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

FilePath::FilePath( const FilePath& rhs )
{
   m_relativePath= rhs.m_relativePath;
}

///////////////////////////////////////////////////////////////////////////////

bool FilePath::isSubPath( const FilePath& path ) const
{
   return ( m_relativePath.find( path.m_relativePath ) != std::string::npos );
}

///////////////////////////////////////////////////////////////////////////////

std::string operator+( const char* str, const FilePath& path )
{
   return str + path.m_relativePath;
}

///////////////////////////////////////////////////////////////////////////////

std::string FilePath::extractExtension() const
{
   return Filesystem::extractExtension( m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

void FilePath::extractDir( FilePath& outDir ) const
{
   outDir.m_relativePath = Filesystem::extractDir( m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

void FilePath::leaveDir(  unsigned int levels, FilePath& outDirectory ) const
{
   Filesystem::leaveDir( m_relativePath, levels, outDirectory.m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

std::string FilePath::extractNodeName() const
{
   return Filesystem::extractNodeName( m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

void FilePath::changeFileExtension( const std::string& newExtension, FilePath& outPath ) const
{
   if ( newExtension.empty() )
   {
      // nothing to change
      outPath.m_relativePath = m_relativePath;
      return;
   }

   std::size_t pos = m_relativePath.find_last_of( "." );
   if ( pos != std::string::npos )
   {
      outPath.m_relativePath = m_relativePath.substr( 0, pos + 1 ) + newExtension;
   }
   else
   {
      outPath.m_relativePath = m_relativePath + "." + newExtension;
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string FilePath::toAbsolutePath( const Filesystem& fs ) const
{
   return fs.getCurrRoot() + m_relativePath;
}

///////////////////////////////////////////////////////////////////////////////

OutStream& operator<<( OutStream& serializer, FilePath& path )
{
   serializer << path.m_relativePath;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

InStream& operator>>( InStream& serializer, FilePath& path )
{
   serializer >> path.m_relativePath;
   return serializer;
}

///////////////////////////////////////////////////////////////////////////////
