#pragma once

/// @file   TamyEditor\ResourcesBrowser.h
/// @brief  viewer allowing to browse through resources

#include "core\Component.h"
#include "core\Filesystem.h"
#include "core\GenericFactory.h"
#include "tamyeditor.h"
#include <QObject>
#include <QTreeWidgetItem>
#include <QPoint>
#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class QTreeWidget;
class QPushButton;
class MainAppComponent;

///////////////////////////////////////////////////////////////////////////////

class ResourcesBrowser : public QObject, 
                         public Component< TamyEditor >,
                         public FilesystemListener,
                         public FilesystemScanner,
                         public GenericFactory< Resource, ResourceEditor >
{
   Q_OBJECT

private:
   /**
    * An item describing an fs node in the tree structure
    */
   class FSTreeEntry : public QTreeWidgetItem 
   {
   private:
      std::string    m_fsNodeName;
      bool           m_isDir;

   public:
      /**
       * Constructor creating the root node.
       *
       * @param hostTree
       * @param fs         file system from which we can access entry icons
       */
      FSTreeEntry( QTreeWidget* hostTree, const Filesystem& fs );

      /**
       * Constructor that attaches a new sub node to a parent.
       *
       * @param nodeName
       * @param isDir
       * @param parent
       * @param fs         file system from which we can access entry icons
       */
      FSTreeEntry( const std::string& nodeName, bool isDir, QTreeWidgetItem* parent, const Filesystem& fs  );

      /**
       * Removes all node's children.
       */
      void clear();

      /**
       * Returns a relative path leading to this fs node.
       */
      std::string getRelativePath() const;

      /**
       * Searches for a child entry with the specified name.
       *
       * @param nodeName
       */
      FSTreeEntry* find( const std::string& nodeName );

      /**
       * Tells whether the node corresponds to a directory.
       */
      inline bool isDir() const { return m_isDir; }

   private:
      void setEntryIcon( const Filesystem& fs );
      void setEntryName( const Filesystem& fs );
      void setEntrySize( const Filesystem& fs );
   };

private:
   TamyEditor*          m_mgr;
   QTreeWidget*         m_fsTree;
   FSTreeEntry*         m_rootDir;
   QPushButton*         m_toggleFileTypesViewBtn;
   bool                 m_viewResourcesOnly;

   ResourcesManager*    m_rm;
   MainAppComponent*    m_mainApp;

public:
   /**
    * Constructor.
    */
   ResourcesBrowser();

   // -------------------------------------------------------------------------
   // Component initialization
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );

   // -------------------------------------------------------------------------
   // FilesystemListener implementation
   // -------------------------------------------------------------------------
   void onDirChanged( const std::string& dir );

   // ----------------------------------------------------------------------
   // FilesystemScanner implementation
   // ----------------------------------------------------------------------
   void onDirectory( const std::string& name );
   void onFile( const std::string& name );

public slots:
   void editResource( QTreeWidgetItem* item, int column );
   void toggleFilesFiltering( bool = false );

private:
   void initializeEditors();
   void initUI( TamyEditor& mgr );
   void refresh( const std::string& rootDir = "/" );
   FSTreeEntry* find( const std::string& dir );
};

///////////////////////////////////////////////////////////////////////////////
