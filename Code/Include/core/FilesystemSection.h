/// @file   core/FilesystemSection.h
/// @brief  a section of the filesystem that filters out all directories except for the selected few
#pragma once

#include <vector>
#include "core/Filesystem.h"


///////////////////////////////////////////////////////////////////////////////

class FilePath;

///////////////////////////////////////////////////////////////////////////////

/**
 * A section of the filesystem that filters out all directories except for the selected few.
 */
class FilesystemSection : public FilesystemListener
{
private:
   Filesystem&                            m_filesystem;

   std::vector< FilePath >                m_allowedDirs;
   std::vector< FilesystemListener* >     m_listeners;

public:
   /**
    * Constructor.
    *
    * @param filesystem       host filesystem
    */
   FilesystemSection( Filesystem& filesystem );
   ~FilesystemSection();

   /**
    * Tells whether the node at the specified path can be modified.
    */
   bool isMember( const FilePath& path ) const;

   /**
    * Adds a new directory to the section filter.
    *
    * @param path
    */
   void addDirectory( const FilePath& path );

   /**
    * Removes a directory from the section filter. If it's a parent directory
    * to several defined section directories, it will cause all of them to be removed.
    *
    * @param path
    */
   void removeDirectory( const FilePath& path );

   /**
    * Returns a list of section directories.
    */
   inline const std::vector< FilePath >& getSectionDirectories() const { return m_allowedDirs; }

   // -------------------------------------------------------------------------
   // Listeners management
   // -------------------------------------------------------------------------
   /**
    * Attaches a filesystem listener to the section.
    *
    * @param listener
    */
   void attach( FilesystemListener& listener );

   /**
    * Detaches a filesystem listener from the section.
    *
    * @param listener
    */
   void detach( FilesystemListener& listener );

   // -------------------------------------------------------------------------
   // FilesystemListener implementation
   // -------------------------------------------------------------------------
   void onFileEdited( const FilePath& path );
   void onFileRemoved( const FilePath& path );
   void onDirAdded( const FilePath& dir );
   void onDirRemoved( const FilePath& dir );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A special scanner that allows you to wrap your custom scanner interface and scan
 * an arbitrary filesystem with it while filtering out anything that doesn't isn't
 * a part of the specified filesystem section.
 */
class FilesystemSectionScanner : public FilesystemScanner
{
private:
   const FilesystemSection&   m_scannedSection;

public:
   /**
    * Constructor.
    *
    * @param scannedSection
    */
   FilesystemSectionScanner( const FilesystemSection& scannedSection );

   // -------------------------------------------------------------------------
   // FilesystemScanner implementation
   // -------------------------------------------------------------------------
   void onDirectory( const FilePath& path );
   void onFile( const FilePath& path );

protected:
   /**
    * Called when a directory that's part of the scanned section is encountered.
    *
    * @param path
    */
   virtual void onSectionDirectory( const FilePath& path ) = 0;

   /**
    * Called when a file that's part of the scanned section is encountered.
    *
    * @param path
    */
   virtual void onSectionFile( const FilePath& path ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
