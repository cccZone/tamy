#include "core.h"
#include "core/FilesystemUtils.h"
#include "core/StringUtils.h"
#include "core/types.h"
#include <vector>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

std::string FilesystemUtils::extractDir( const std::string& fileName )
{
   std::size_t lastSlash = fileName.find_last_of( "/" );
   if ( lastSlash == std::string::npos )
   {
      // the name doesn't contain any slash - therefore there's no dir
      // definition in the path
      return "";
   }
   else if ( lastSlash < fileName.length() - 1 )
   {
      // this is a file name - we can just cut the name
      // at the slash position and take the right splice
      return fileName.substr( 0, lastSlash + 1 );
   }
   else
   {
      // this is a directory ( ends with a slash ) - we need to find
      // a previous slash and cut the name up till there
      std::size_t prevSlash = fileName.find_last_of( "/", lastSlash - 1 );

      if ( lastSlash == 0 || prevSlash == std::string::npos )
      {
         // no directory is specified
         return "";
      }
      else 
      {
         return fileName.substr( 0, prevSlash + 1 );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemUtils::normalize( const std::string& fileName, std::string& outFileName )
{
   outFileName = "";

   {
      int lastSlash = 0;
      int nextSlash = fileName.find_first_of( "\\" );

      while( (std::size_t)nextSlash != std::string::npos )
      {
         outFileName += fileName.substr( lastSlash, nextSlash - lastSlash );
         outFileName += "/";

         lastSlash = nextSlash + 1;
         nextSlash = fileName.find_first_of( "\\", lastSlash );
      }

      outFileName += fileName.substr( lastSlash, fileName.length() );
   }

   // look for subsequent slashes and eliminate them as well
   {
      int nextDoubleSlash = outFileName.find( "//" );
      while( (std::size_t)nextDoubleSlash != std::string::npos )
      {
         outFileName.replace( nextDoubleSlash, 1, "", 0 );
         nextDoubleSlash = outFileName.find( "//" );
      }
   }

   // last but not least - the name HAS TO start with a slash - if it doesn't, add it
   {
      bool isPathAbsolute = outFileName.length() >= 2 && outFileName.c_str()[1] == ':';
      bool isInvalidRelative = outFileName.empty() || outFileName.c_str()[0] != '/';
      if ( !isPathAbsolute && isInvalidRelative )
      {
         outFileName = std::string( "/" ) + outFileName;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemUtils::leaveDir( const std::string& dir, unsigned int levels, std::string& outDirectory )
{
   // first - remove the last slash, if any's appended
   std::size_t lastSlash = dir.find_last_of( "/" );
   if ( lastSlash == dir.length() - 1 )
   {
      outDirectory = dir.substr( 0, dir.length() - 1 );
   }
   else
   {
      outDirectory = dir;
   }

   // now remove the proper amount of directories in the path
   while( levels > 0 )
   {
      std::size_t lastSlash = outDirectory.find_last_of( "/" );
      if ( lastSlash == std::string::npos )
      {
         // the name doesn't contain any slash - therefore there's no dir
         // definition in the path
         return;
      }

      outDirectory = outDirectory.substr( 0, lastSlash );
      --levels;
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string FilesystemUtils::extractNodeName( const std::string& fileName )
{
   std::size_t lastSlash = fileName.find_last_of( "/" );
   if ( lastSlash == std::string::npos )
   {
      // the name doesn't contain any slash - therefore there's no dir
      // definition in the path
      return fileName;
   }
   else if ( lastSlash < fileName.length() - 1 )
   {
      // this is a file name - we can just cut the name
      // at the slash position and take the right splice
      return fileName.substr( lastSlash + 1 );
   }
   else
   {
      // this is a directory ( ends with a slash ) - we need to find
      // a previous slash and cut the name up till there
      std::size_t prevSlash = fileName.find_last_of( "/", lastSlash - 1 );

      if ( lastSlash == 0 || prevSlash == std::string::npos )
      {
         // no directory is specified
         return fileName;
      }
      else 
      {
         return fileName.substr( prevSlash + 1 );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string FilesystemUtils::extractExtension( const std::string& fileName ) 
{
   std::size_t pos = fileName.find_last_of( "." );
   if ( pos != std::string::npos )
   {
      return fileName.substr( pos + 1 );
   }
   else
   {
      return "";
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemUtils::mkdir( const char* path )
{
   if ( !path )
   {
      return;
   }

   std::vector< std::string > pathElems;
   StringUtils::tokenize( path, "/\\", pathElems );

   uint pathElemsCount = pathElems.size();
   if ( pathElemsCount <= 1 )
   {
      // not much to create here
      return;
   }

   std::string currPath = pathElems[0];
   for ( uint i = 1; i < pathElemsCount; ++i )
   {
      currPath += "/" + pathElems[i];
      CreateDirectory( currPath.c_str(), NULL );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool FilesystemUtils::copyFile( const char* sourcePath, const char* destPath, bool createDir )
{
   if ( createDir )
   {
      std::string normalizedDestPath;
      normalize( destPath, normalizedDestPath );
      std::string dirPath = extractDir( destPath );
      mkdir( dirPath.c_str() );
   }

   BOOL result = CopyFile( sourcePath, destPath, true );
   return result > 0;
}

///////////////////////////////////////////////////////////////////////////////
