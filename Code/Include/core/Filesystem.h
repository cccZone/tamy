#pragma once

/// @file   core\Filesystem.h
/// @brief  filesystem manager

#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class File;

///////////////////////////////////////////////////////////////////////////////

/**
 * A manager of the file system that can be used to access 
 * external files.
 */
class Filesystem 
{
private:
   std::string m_rootDir;

public:
   /**
    * Constructor.
    *
    * @param rootDir    the initial root directory
    */
   Filesystem(const std::string& rootDir = "");

   /**
    * This manager maps the file system to a specific directory's contents.
    * This method allows to define that directory. From this point on 
    * every file name will be considered relative to this root dir.
    *
    * @param rootDir    name of the file system root directory
    */
   void changeRootDir(const std::string& rootDir);

   /**
    * Returns the current filesystem root directory.
    *
    * @return           filesystem root directory name
    */
   const std::string& getCurrRoot() const;

   /**
    * A quick way to check if the given file exists in the file system.
    *
    * @param fileName   name of the file we want to find
    * @return           'true' if the file exists in the file system,
    *                   'false' otherwise
    */
   bool doesExist(const std::string& fileName) const;

   /**
    * The method opens a file with specific access privileges.
    *
    * @param fileName   name of the file we want to open
    * @param mode       access mode
    */
   File* open(const std::string& fileName, 
              const std::ios_base::openmode mode = std::ios_base::in) const;

   // -------------------------------------------------------------------------
   // Utility methods
   // -------------------------------------------------------------------------
   /**
    * Extracts an extension of the specified filename.
    *
    * @param fileName
    */
   static std::string extractExtension( const std::string& fileName );

   /**
    * Changes the extension on a filename, and returns it in a brand new string.
    *
    * @param fileName
    * @param newExtension
    */
   static std::string changeFileExtension( const std::string& fileName, const std::string& newExtension );

   /**
    * Converts the specified absolute path to the filesystem-relative path.
    *
    * @param absoluteFilePath
    */
   std::string toRelativePath( const std::string& absoluteFilePath ) const;

private:
   void tokenize(const std::string& str, 
      std::vector<std::string>& output,
      const std::string& tokens) const;
};

///////////////////////////////////////////////////////////////////////////////
