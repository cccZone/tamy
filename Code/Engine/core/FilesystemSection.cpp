#include "core/FilesystemSection.h"
#include "core/Filesystem.h"
#include "core/FilePath.h"
#include "core/types.h"


///////////////////////////////////////////////////////////////////////////////

FilesystemSection::FilesystemSection( Filesystem& filesystem )
   : m_filesystem( filesystem )
{
   m_filesystem.attach( *this );
}

///////////////////////////////////////////////////////////////////////////////

FilesystemSection::~FilesystemSection()
{
   m_filesystem.detach( *this );
}

///////////////////////////////////////////////////////////////////////////////

bool FilesystemSection::isMember( const FilePath& path ) const
{
   uint count = m_allowedDirs.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( path.isSubPath( m_allowedDirs[i] ) )
      {
         return true;
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSection::addDirectory( const FilePath& path )
{
   // check the path's uniqueness
   uint count = m_allowedDirs.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_allowedDirs[i] == path )
      {
         return;
      }
   }

   // remove all previously defined directories that are children of the added directory
   for ( int i = count - 1; i >= 0; --i )
   {
      if ( m_allowedDirs[i].isSubPath( path ) )
      {
         m_allowedDirs.erase( m_allowedDirs.begin() + i );
      }
   }

   m_allowedDirs.push_back( path );

   // inform the listeners about the new directory being attached
   {
      uint count = m_listeners.size();
      for ( uint i = 0; i < count; ++i )
      {
         m_listeners[i]->onDirAdded( path );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSection::removeDirectory( const FilePath& path )
{
   // remove all directories that are either the specified dir, or are its children
   bool wasDirRemoved = false;
   uint count = m_allowedDirs.size();
   for ( int i = count - 1; i >= 0; --i )
   {
      if ( path == m_allowedDirs[i] || m_allowedDirs[i].isSubPath( path ) )
      {
         m_allowedDirs.erase( m_allowedDirs.begin() + i );
         wasDirRemoved = true;
         break;
      }
   }

   // inform the listeners about the new directory being attached
   if ( wasDirRemoved )
   {
      uint count = m_listeners.size();
      for ( uint i = 0; i < count; ++i )
      {
         m_listeners[i]->onDirRemoved( path );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSection::attach( FilesystemListener& listener )
{
   // check the listener's uniqueness
   uint count = m_listeners.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_listeners[i] == &listener )
      {
         return;
      }
   }

   m_listeners.push_back( &listener );

   // inform the listener about all kept directories
   {
      uint dirsCount = m_allowedDirs.size();
      for ( uint i = 0; i < dirsCount; ++i )
      {
         listener.onDirAdded( m_allowedDirs[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSection::detach( FilesystemListener& listener )
{
   bool wasDetached = false;
   uint count = m_listeners.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( m_listeners[i] == &listener )
      {
         m_listeners.erase( m_listeners.begin() + i );
         wasDetached = true;
         break;
      }
   }

   // inform the listener that all directories from this section have been detached
   if ( wasDetached )
   {
      uint dirsCount = m_allowedDirs.size();
      for ( uint i = 0; i < dirsCount; ++i )
      {
         listener.onDirRemoved( m_allowedDirs[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSection::onFileEdited( const FilePath& path )
{
   if ( !isMember( path ) )
   {
      return;
   }

   // pass the message on to the listeners
   uint count = m_listeners.size();
   for ( uint i = 0; i < count; ++i )
   {
      m_listeners[i]->onFileEdited( path );
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSection::onFileRemoved( const FilePath& path )
{
   if ( !isMember( path ) )
   {
      return;
   }

   // pass the message on to the listeners
   uint count = m_listeners.size();
   for ( uint i = 0; i < count; ++i )
   {
      m_listeners[i]->onFileRemoved( path );
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSection::onDirAdded( const FilePath& dir )
{
   if ( !isMember( dir ) )
   {
      return;
   }

   // pass the message on to the listeners
   uint count = m_listeners.size();
   for ( uint i = 0; i < count; ++i )
   {
      m_listeners[i]->onDirAdded( dir );
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSection::onDirRemoved( const FilePath& dir )
{
   if ( !isMember( dir ) )
   {
      return;
   }

   // pass the message on to the listeners
   uint count = m_listeners.size();
   for ( uint i = 0; i < count; ++i )
   {
      m_listeners[i]->onDirRemoved( dir );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

FilesystemSectionScanner::FilesystemSectionScanner( const FilesystemSection& scannedSection )
   : m_scannedSection( scannedSection )
{}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSectionScanner::onDirectory( const FilePath& path )
{
   // check if the directory is allowed
   if ( m_scannedSection.isMember( path ) )
   {
      onSectionDirectory( path );
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemSectionScanner::onFile( const FilePath& path )
{
   // check if the directory is allowed
   if ( m_scannedSection.isMember( path ) )
   {
      onSectionFile( path );
   }
}

///////////////////////////////////////////////////////////////////////////////
