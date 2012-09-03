#include "core/FilePath.h"
#include "core/InStream.h"
#include "core/OutStream.h"
#include "core/Filesystem.h"
#include "core/FilesystemUtils.h"
#include "core/StringUtils.h"
#include "core/Profiler.h"


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
   FilesystemUtils::normalize( path, m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

FilePath::FilePath( const FilePath& rhs )
{
   m_relativePath = rhs.m_relativePath;
}

///////////////////////////////////////////////////////////////////////////////

void FilePath::set( const std::string& path )
{
   FilesystemUtils::normalize( path, m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

bool FilePath::isSubPath( const FilePath& path ) const
{
   return ( m_relativePath.find( path.m_relativePath ) != std::string::npos );
}

///////////////////////////////////////////////////////////////////////////////

bool FilePath::isParent( const FilePath& parentCandidatePath ) const
{
   return ( parentCandidatePath.m_relativePath.find( m_relativePath ) != std::string::npos );
}

///////////////////////////////////////////////////////////////////////////////

std::string operator+( const char* str, const FilePath& path )
{
   return str + path.m_relativePath;
}

///////////////////////////////////////////////////////////////////////////////

std::string FilePath::extractExtension() const
{
   return FilesystemUtils::extractExtension( m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

void FilePath::extractDir( FilePath& outDir ) const
{
   outDir.m_relativePath = FilesystemUtils::extractDir( m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

void FilePath::leaveDir(  unsigned int levels, FilePath& outDirectory ) const
{
   FilesystemUtils::leaveDir( m_relativePath, levels, outDirectory.m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////

std::string FilePath::extractNodeName() const
{
   return FilesystemUtils::extractNodeName( m_relativePath );
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

void FilePath::toAbsolutePath( const Filesystem& fs, char* outAbsPath ) const
{
   int firstPartLen = fs.getCurrRoot().length();
   int secondPartLen = m_relativePath.length();

   const char* firstPathPart = fs.getCurrRoot().c_str();
   const char* secondPathPart = m_relativePath.c_str();
   memcpy( outAbsPath, firstPathPart, firstPartLen );
   memcpy( outAbsPath + firstPartLen, secondPathPart, secondPartLen );
   outAbsPath[firstPartLen + secondPartLen ] = 0;
}

///////////////////////////////////////////////////////////////////////////////

OutStream& operator<<( OutStream& serializer, const FilePath& path )
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

void FilePath::getElements( std::vector< std::string >& outPathElements ) const
{
   StringUtils::tokenize( m_relativePath, "/", outPathElements );
}

///////////////////////////////////////////////////////////////////////////////
