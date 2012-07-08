/// @file   core\Filesystem.h
/// @brief  file system manager
#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>


///////////////////////////////////////////////////////////////////////////////

class File;
class FilePath;

///////////////////////////////////////////////////////////////////////////////

/**
 * A listener gets informed about the changes occurring within a file system.
 */
class FilesystemListener
{
public:
   virtual ~FilesystemListener() {}

   /**
    * Called when a file in the file system changes ( is either created, or its contents changes ).
    *
    * @param path    file path
    */
   virtual void onFileEdited( const FilePath& path ) = 0;

   /**
    * Called when a file in the file system is removed.
    *
    * @param path    file path
    */
   virtual void onFileRemoved( const FilePath& path ) = 0;

   /**
    * Called when a directory was added.
    *
    * @param dir     directory that was added
    */
   virtual void onDirAdded( const FilePath& dir ) = 0;

   /**
    * Called when a directory was removed.
    *
    * @param dir     directory that was removed
    */
   virtual void onDirRemoved( const FilePath& dir ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class FilesystemScanner
{
public:
   virtual ~FilesystemScanner() {}

   /**
    * Called when a directory is encountered during the scanning process.
    * The name will always end with a '/' character.
    *
    * @param name    relative path to the directory (along with that directory name)
    */
   virtual void onDirectory( const FilePath& name ) = 0;

   /**
    * Called when a file is encountered during the scanning process.
    *
    * @param name    relative path to the file (along with that file name)
    */
   virtual void onFile( const FilePath& name ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A manager of the file system that can be used to access 
 * external files.
 */
class Filesystem
{
private:
   typedef std::map< std::string, std::string > Shortcuts;
   typedef std::vector< FilesystemListener* >   Listeners;

private:
   std::string          m_rootDir;
   Shortcuts            m_shortcuts;
   Listeners            m_listeners;

public:
   /**
    * Constructor.
    *
    * @param rootDir    the initial root directory
    */
   Filesystem( const std::string& rootDir = "" );

   /**
    * This manager maps the file system to a specific directory's contents.
    * This method allows to define that directory. From this point on 
    * every file name will be considered relative to this root dir.
    *
    * @param rootDir    name of the file system root directory
    */
   void changeRootDir( const std::string& rootDir );

   /**
    * Returns the current filesystem root directory.
    *
    * @return           filesystem root directory name
    */
   const std::string& getCurrRoot() const;

   /**
    * Checks if the specified path points to a directory, or a regular file.
    *
    * @param path
    */
   bool isDir( const FilePath& path ) const;

   /**
    * A quick way to check if the given file exists in the file system.
    *
    * @param fileName   name of the file we want to find
    * @return           'true' if the file exists in the file system,
    *                   'false' otherwise
    */
   bool doesExist( const FilePath& fileName ) const;

   /**
    * The method opens a file with specific access privileges.
    *
    * @param fileName   name of the file we want to open
    * @param mode       access mode
    */
   File* open( const FilePath& fileName, const std::ios_base::openmode mode = std::ios_base::in ) const;

   /**
    * Creates a new directory.
    *
    * @param dirName    relative path to the directory we want to create.
    */
   void mkdir( const FilePath& dirName ) const;

   /**
    * Removes an fs element specified by the path from the file system.
    *
    * @param path       path to an element we want to remove
    *
    * @return           'true' if the element was successfully removed, 'false' otherwise
    */
   bool remove( const FilePath& path ) const;

   // -------------------------------------------------------------------------
   // Listeners management
   // -------------------------------------------------------------------------
   /**
    * Attaches a new listener.
    *
    * @param listener.
    */
   void attach( FilesystemListener& listener );

   /**
    * Detaches a listener.
    *
    * @param listener.
    */
   void detach( FilesystemListener& listener );

   // -------------------------------------------------------------------------
   // Shortcuts
   // -------------------------------------------------------------------------
   
   /**
    * A shortcut is a concise id that can be assigned to a relative path.
    *
    * Querying for a shortcut, we'll always get an absolute path,
    * concatenated from the file system's current root directory and the relative
    * path assigned to the shortcut.
    *
    * Assigning an existing shortcut a new value overwrites the previous setting.
    *
    * @param shortcut
    * @param relativePath
    */
   void setShortcut( const std::string& shortcut, const std::string& relativePath );

   /**
    * Returns the absolute path assigned to the specified shortcut.
    *
    * @param shortcut
    */
   std::string getShortcut( const std::string& shortcut ) const;

   // -------------------------------------------------------------------------
   // Utils
   // -------------------------------------------------------------------------
   /**
    * Converts the specified absolute path to the file system relative path.
    *
    * @param absoluteFilePath
    */
   std::string toRelativePath( const std::string& absoluteFilePath ) const;

   /**
    * Converts the specified relative path to the file system absolute path.
    *
    * @param relativeFilePath
    */
   std::string toAbsolutePath( const std::string& relativeFilePath ) const;

   /**
    * Normalizes the file path - setting proper directories separators etc.
    *
    * @param fileName
    * @param outFileName
    */
   static void normalize( const std::string& fileName, std::string& outFileName );

   /**
    * Extracts the name of a directory in which the specified file is located.
    *
    * @param fileName
    */
   static std::string extractDir( const std::string& fileName );

   /**
    * The method allows to move up in the directories hierarchy by the specified
    * amount of levels.
    *
    * i.e.: A call like:
    *    leaveDir( "d:/dirA/dirB/dirC/dirD", 2, outDir );
    * initializes outDir with:
    *    "d:/dirA/dirB"
    *
    * @param dir
    * @param levels
    * @param outDirectory
    */
   static void leaveDir( const std::string& dir, unsigned int levels, std::string& outDirectory );

   /**
    * Just as the 'extractDir' method extracts a name of a directory
    * in which the specified file is located, this method extracts
    * the sole name of the file.
    *
    * @param fileName   path to a file along with that file name.
    */
   static std::string extractNodeName( const std::string& fileName );

   /**
    * Extracts an extension of the specified filename.
    *
    * @param fileName
    */
   static std::string extractExtension( const std::string& fileName );

   /**
    * The method scans the file system, starting from the specified root
    * directory, and informs via the FilesystemScanner interface about its finding.
    *
    * @param rootDir          directory from which the scanning should begin
    * @param scanner
    * @param recursive        use recursive search through the directories tree
    */
   void scan( const FilePath& rootDir, FilesystemScanner& scanner, bool recursive = true ) const;

protected:
   friend class File;

   /**
    * Called when a file that was opened for edition gets closed.
    *
    * @param fileName      name of the edited file
    */
   void onFileEditionCompleted( const FilePath& fileName ) const;

private:
   void notifyFileEditedChange( const FilePath& path ) const;
   void notifyFileRemovedChange( const FilePath& path ) const;
   void notifyDirChange( const FilePath& dir, bool wasAdded ) const;
};

///////////////////////////////////////////////////////////////////////////////
