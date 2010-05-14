#include "core\Filesystem.h"
#include "core\File.h"
#include "core\StringUtils.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

Filesystem::Filesystem(const std::string& rootDir)
{
   changeRootDir(rootDir);
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::changeRootDir(const std::string& rootDir)
{
   m_rootDir = rootDir;

   std::size_t endingSlashPos = m_rootDir.find_last_of("/\\");
   if (endingSlashPos != (m_rootDir.length() - 1))
   {
      m_rootDir += "/";
   }
}

///////////////////////////////////////////////////////////////////////////////

const std::string& Filesystem::getCurrRoot() const
{
   return m_rootDir;
}

///////////////////////////////////////////////////////////////////////////////

bool Filesystem::doesExist(const std::string& fileName) const
{
   File* file = NULL;
   try
   {
      file = open(fileName);
   }
   catch (std::runtime_error&)
   {
      file = NULL;
   }

   bool exists = (file != NULL);
   delete file;

   return exists;
}

///////////////////////////////////////////////////////////////////////////////

File* Filesystem::open(const std::string& fileName, 
                       const std::ios_base::openmode mode) const
{
   std::string fullFileName = m_rootDir + fileName;
   File* file = new File(fullFileName, mode);

   // we're writing to a file - that changes the fs contents, therefore
   // send out the notifications
   if ( ( mode & std::ios_base::out ) == std::ios_base::out )
   {
      std::string dir = extractDir( fileName );
      notifyDirChange( dir );
   }

   return file;
}

///////////////////////////////////////////////////////////////////////////////

std::string Filesystem::extractExtension( const std::string& fileName )
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

std::string Filesystem::extractDir( const std::string& fileName )
{
   std::size_t pos = fileName.length();
   std::size_t len = fileName.length() - 1;
   while( true )
   {
      pos = fileName.find_last_of( "/", pos - 1 );
      if ( pos == std::string::npos || pos == 0 )
      {
         return "";
      }

      if ( pos < len ) 
      {
         return fileName.substr( 0, pos + 1 );
      }
   }

   return "";
}

///////////////////////////////////////////////////////////////////////////////

std::string Filesystem::extractNodeName( const std::string& fileName )
{
   std::size_t pos = fileName.length();
   std::size_t len = fileName.length() - 1;
   while( true )
   {
      pos = fileName.find_last_of( "/", pos - 1 );
      if ( pos == std::string::npos || pos == 0)
      {
         return fileName;
      }

      if ( pos < len ) 
      {
         return fileName.substr( pos + 1 );
      }
   }

   return fileName;
}

///////////////////////////////////////////////////////////////////////////////

std::string Filesystem::changeFileExtension( const std::string& fileName, 
                                             const std::string& newExtension )
{
   std::size_t pos = fileName.find_last_of( "." );
   if ( pos != std::string::npos )
   {
      return fileName.substr( 0, pos + 1 ) + newExtension;
   }
   else
   {
      return fileName + "." + newExtension;
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string Filesystem::toRelativePath(const std::string& absoluteFilePath) const
{
   // tokenize both the filename and the root dir
   std::vector<std::string> fileNameTokens;
   std::vector<std::string> rootDirTokens;

   StringUtils::tokenize( absoluteFilePath, "/", fileNameTokens );
   StringUtils::tokenize( m_rootDir, "/", rootDirTokens );

   // if the root dir path contains more or equal amount of nodes
   // as the filename, file can't possibly be located inside that directory
   if (rootDirTokens.size() >= fileNameTokens.size())
   {
      return "";
   }

   // compare all the nodes from the root directory to those on the file
   // to make sure the file is indeed located inside the root director
   unsigned int rootDirTokensCount = rootDirTokens.size();
   for (unsigned int i = 0; i < rootDirTokensCount; ++i)
   {
      if (rootDirTokens[i] != fileNameTokens[i])
      {
         return "";
      }
   }

   // assemble a file name relative to the root directory
   std::string relativeFileName = "";
   unsigned int fileNameTokensCount = fileNameTokens.size();
   for (unsigned int i = rootDirTokensCount; i < fileNameTokensCount; ++i)
   {
      relativeFileName += "/" + fileNameTokens[i];
   }

   // if the path ended with a backslash (meaning that it was a path to a directory),
   // finish it with one as well
   if ( absoluteFilePath[ absoluteFilePath.length() - 1 ] == '/' )
   {
      relativeFileName += "/";
   }
   return relativeFileName;
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::setShortcut( const std::string& shortcut, const std::string& relativePath )
{
   Shortcuts::iterator it = m_shortcuts.find( shortcut );
   if ( it != m_shortcuts.end() )
   {
      it->second = relativePath;
   }
   else
   {
      m_shortcuts.insert( std::make_pair( shortcut, relativePath ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string Filesystem::getShortcut( const std::string& shortcut ) const
{
   Shortcuts::const_iterator it = m_shortcuts.find( shortcut );
   if ( it != m_shortcuts.end() )
   {
      return m_rootDir + it->second;
   }
   else
   {
      return "";
   }
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::attach( FilesystemListener& listener )
{
   m_listeners.insert( &listener );
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::detach( FilesystemListener& listener )
{
   Listeners::iterator it = m_listeners.find( &listener );
   if ( it != m_listeners.end() )
   {
      m_listeners.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::notifyDirChange( const std::string& dir ) const
{
   for ( Listeners::const_iterator it = m_listeners.begin();
         it != m_listeners.end(); ++it )
   {
      ( *it )->onDirChanged( dir );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::scan( const std::string& rootDir, FilesystemScanner& scanner ) const
{
   std::vector< std::string > pathsStack;
   pathsStack.push_back( m_rootDir + rootDir + "/" );

   while( !pathsStack.empty() )
   {
      std::string currPath = pathsStack.back();
      pathsStack.pop_back();

      WIN32_FIND_DATA findFileData;
      HANDLE hFind;
      bool result = true;
      for ( hFind = FindFirstFile( ( currPath + "*" ).c_str(), &findFileData );
            hFind != INVALID_HANDLE_VALUE && result; 
            result = FindNextFile( hFind, &findFileData ) )
      {
         std::string name = findFileData.cFileName;
         if ( ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
         {
            // we found a directory
            if ( name != "." && name != ".." )
            {
               std::string dirName = currPath + name + "/";
               scanner.onDirectory( dirName );
               pathsStack.push_back( dirName );
            }
         }
         else
         {
            // we found a file
            scanner.onFile( currPath + name );
         }
      }

      FindClose( hFind );
   }
}

///////////////////////////////////////////////////////////////////////////////
