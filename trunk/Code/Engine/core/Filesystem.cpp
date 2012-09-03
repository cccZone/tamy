#include "core\Filesystem.h"
#include "core\FilesystemUtils.h"
#include "core\File.h"
#include "core\FilePath.h"
#include "core\StringUtils.h"
#include <stdexcept>
#include <algorithm>
#include <list>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class HierarchicalFilesDeleter : public FilesystemScanner
   {
   private:
      struct DeletionDesc
      {
         std::string path;
         bool        isDir;

         DeletionDesc( const std::string& _path, bool _isDir )
            : path( _path )
            , isDir( _isDir )
         {}
      };

   private:
      std::string                m_rootDir;
      std::list< DeletionDesc >  m_deletionList;
      
   public:
      HierarchicalFilesDeleter( const std::string& rootDir )
         : m_rootDir( rootDir )
      {}

      void perform()
      {
         for ( std::list< DeletionDesc >::const_iterator it = m_deletionList.begin();
            it != m_deletionList.end(); ++it )
         {
            const DeletionDesc& desc = *it;
            if ( desc.isDir )
            {
               RemoveDirectory( desc.path.c_str() );
            }
            else
            {
               DeleteFile( desc.path.c_str() );
            }
         }
      }

      // ----------------------------------------------------------------------
      // FilesystemScanner
      // ----------------------------------------------------------------------
      void onDirectory( const std::string& name ) 
      { 
         m_deletionList.push_front( DeletionDesc( m_rootDir + name, true ) ); 
      }

      void onFile( const std::string& name )
      {
         m_deletionList.push_front( DeletionDesc( m_rootDir + name, false ) ); 
      }
   };
};

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

bool Filesystem::isDir( const FilePath& fileName ) const
{
   if ( fileName.empty() )
   {
      // yup - it's the root directory
      return true;
   }

   std::string absPathStr = fileName.toAbsolutePath( *this );
   DWORD attribs = GetFileAttributesA( absPathStr.c_str() );
   return ( ( attribs & FILE_ATTRIBUTE_DIRECTORY ) == INVALID_FILE_ATTRIBUTES );
}

///////////////////////////////////////////////////////////////////////////////

bool Filesystem::doesExist( const FilePath& fileName ) const
{
   if ( fileName.empty() )
   {
      return false;
   }

   std::string absPathStr = fileName.toAbsolutePath( *this );
   DWORD attribs = GetFileAttributesA( absPathStr.c_str() );
   return attribs != INVALID_FILE_ATTRIBUTES;
}

///////////////////////////////////////////////////////////////////////////////

File* Filesystem::open( const FilePath& fileName, const std::ios_base::openmode mode ) const
{
   File* file = new File( *this, fileName, mode );
   if ( !file->isOpened() )
   {
      delete file;
      file = NULL;
   }

   return file;
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::onFileEditionCompleted( const FilePath& fileName ) const
{
   notifyFileEditedChange( fileName );
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::mkdir( const FilePath& dirName ) const
{
   std::string absPath = dirName.toAbsolutePath( *this );
   FilesystemUtils::mkdir( absPath.c_str() );

   FilePath parentDir;
   dirName.extractDir( parentDir );
   notifyDirChange( parentDir, true );
}

///////////////////////////////////////////////////////////////////////////////

bool Filesystem::remove( const FilePath& path ) const
{
   std::string absPathStr = path.toAbsolutePath( *this );

   // check file attributes
   DWORD attribs = GetFileAttributesA( absPathStr.c_str() );
   if ( attribs == INVALID_FILE_ATTRIBUTES )
   {
      // the file doesn't seem to exist
      return false;
   }

   // create a double-null terminated string with the absolute path
   // to the element we want to remove
   std::size_t strLen = absPathStr.length() + 2;
   char* absPath = new char[ strLen ];
   memset( absPath, 0, strLen );
   strcpy_s( absPath, strLen, absPathStr.c_str() );
   absPath[ strLen - 2 ] = 0;
   absPath[ strLen - 1 ] = 0;

   // setup a shell operation that will remove the element
   SHFILEOPSTRUCT fileOp;
   ZeroMemory( &fileOp, sizeof( SHFILEOPSTRUCT ) );
   fileOp.pFrom = absPath;
   fileOp.wFunc = FO_DELETE;
   fileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
   int result = SHFileOperation( &fileOp );
   
   // cleanup
   delete [] absPath;

   // send notifications
   if ( result == 0 )
   {
      // operation was successful, so notify the listeners about it
      if ( ( attribs & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
      {
         notifyDirChange( path, false );
      }
      else
      {
         notifyFileRemovedChange( path );
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

std::string Filesystem::toRelativePath(const std::string& absoluteFilePath ) const
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

std::string Filesystem::toAbsolutePath( const std::string& relativeFilePath ) const
{
   return m_rootDir + relativeFilePath;
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
   // check for duplicates
   if ( std::find( m_listeners.begin(), m_listeners.end(), &listener ) != m_listeners.end() )
   {
      // such a listener already exists
      return;
   }

   m_listeners.push_back( &listener );
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::detach( FilesystemListener& listener )
{
   Listeners::iterator it = std::find( m_listeners.begin(), m_listeners.end(), &listener );
   if ( it != m_listeners.end() )
   {
      m_listeners.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::notifyFileEditedChange( const FilePath& path ) const
{
   for ( Listeners::const_iterator it = m_listeners.begin(); it != m_listeners.end(); ++it )
   {
      ( *it )->onFileEdited( path );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::notifyFileRemovedChange( const FilePath& path ) const
{
   for ( Listeners::const_iterator it = m_listeners.begin(); it != m_listeners.end(); ++it )
   {
      ( *it )->onFileRemoved( path );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::notifyDirChange( const FilePath& dir, bool wasAdded ) const
{
   for ( Listeners::const_iterator it = m_listeners.begin(); it != m_listeners.end(); ++it )
   {
      if ( wasAdded )
      {
         ( *it )->onDirAdded( dir );
      }
      else
      {
         ( *it )->onDirRemoved( dir );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Filesystem::scan( const FilePath& rootDir, FilesystemScanner& scanner, bool recursive ) const
{
   std::vector< std::string > pathsStack;
   pathsStack.push_back( rootDir.toAbsolutePath( *this ) + "/" );

   while( !pathsStack.empty() )
   {
      std::string currPath = pathsStack.back();
      pathsStack.pop_back();

      WIN32_FIND_DATA findFileData;
      HANDLE hFind;
      bool result = true;
      for ( hFind = FindFirstFile( ( currPath + "*" ).c_str(), &findFileData ); hFind != INVALID_HANDLE_VALUE && result; result = FindNextFile( hFind, &findFileData ) )
      {
         std::string name = findFileData.cFileName;
         if ( ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
         {
            // we found a directory
            if ( name != "." && name != ".." )
            {
               std::string dirName = currPath + name + "/";
               scanner.onDirectory( FilePath( toRelativePath( dirName ) ) );

               if ( recursive )
               {
                  // this is a recursive search, so add the found directory to the search tree
                  pathsStack.push_back( dirName );
               }
            }
         }
         else
         {
            // we found a file
            scanner.onFile( FilePath( toRelativePath( currPath + name ) ) );
         }
      }

      FindClose( hFind );
   }
}

///////////////////////////////////////////////////////////////////////////////
