#pragma once

/// @file   core\Filesystem.h
/// @brief  filesystem manager

#include <string>


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
};

///////////////////////////////////////////////////////////////////////////////
