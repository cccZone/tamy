/// @file   core/FilePath.h
/// @brief  unified file path representation
#pragma once

#include <string>
#include "core\Class.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class Serializer;

///////////////////////////////////////////////////////////////////////////////

class FilePath
{
private:
   std::string       m_relativePath;

public:
   /**
    * Constructor.
    */
   FilePath();
   ~FilePath();

   /**
    * Constructor.
    *
    * @param path
    */
   FilePath( const std::string& path );

   /**
    * Copy constructor.
    *
    * @param rhs
    */
   FilePath( const FilePath& rhs );

   /**
    * Returns the relative file path as a raw string.
    */
   inline const char* c_str() const { return m_relativePath.c_str(); }

   /**
    * Returns the relative file path.
    */
   inline const std::string& getRelativePath() const { return m_relativePath; }

   /**
    * Assignment operator.
    */
   inline void operator=( const FilePath& rhs ) { m_relativePath = rhs.m_relativePath; }

   /**
    * Conversion operator.
    */
   inline operator const std::string& () const { return m_relativePath; }

   /**
    * Equality operator.
    *
    * @param rhs
    */
   inline bool operator==( const FilePath& rhs ) const { return m_relativePath == rhs.m_relativePath; }

   /**
    * Inequality operator.
    *
    * @param rhs
    */
   bool operator!=( const FilePath& rhs ) const { return m_relativePath != rhs.m_relativePath; }

   /**
    * Comparison operator.
    *
    * @param rhs
    */
   bool operator<( const FilePath& rhs ) const { return m_relativePath < rhs.m_relativePath; }

   /**
    * Allows to concatenate a string with a file path.
    * 
    * @param str
    * @param path
    */
   friend std::string operator+( const char* str, const FilePath& path );

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend Serializer& operator<<( Serializer& serializer, FilePath& path );

   // -------------------------------------------------------------------------
   // Utility methods
   // -------------------------------------------------------------------------

   /**
    * Checks if the specified path is a sub path of this path.
    */
   bool isSubPath( const FilePath& path ) const;

   /**
    * Extracts an extension of the specified filename.
    */
   std::string extractExtension() const;

   /**
    * Extracts the name of a directory in which the specified file is located.
    *
    * @param outDir
    */
   void extractDir( FilePath& outDir ) const;

   /**
    * The method allows to move up in the directories hierarchy by the specified
    * amount of levels.
    *
    * i.e.: A call like:
    *    leaveDir( "d:/dirA/dirB/dirC/dirD", 2, outDir );
    * initializes outDir with:
    *    "d:/dirA/dirB"
    *
    * @param levels
    * @param outDirectory
    */
   void leaveDir( unsigned int levels, FilePath& outDirectory ) const;

   /**
    * Just as the 'extractDir' method extracts a name of a directory
    * in which the specified file is located, this method extracts
    * the sole name of the file.
    *
    * @param fileName   path to a file along with that file name.
    */
   std::string extractNodeName() const;

   /**
    * Changes the extension on a filename, and returns it in a brand new string.
    *
    * @param newExtension
    */
   void changeFileExtension( const std::string& newExtension, FilePath& outPath ) const;

   /**
    * Converts the specified relative path to the file system absolute path.
    *
    * @param fs      filesystem with respect to which we want to transform the path
    */
   std::string toAbsolutePath( const Filesystem& fs) const;

};

///////////////////////////////////////////////////////////////////////////////
