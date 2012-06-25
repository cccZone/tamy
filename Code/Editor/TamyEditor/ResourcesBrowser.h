/// @file   TamyEditor\ResourcesBrowser.h
/// @brief  viewer allowing to browse through resources
#pragma once

#include "core\Filesystem.h"

#include <QDockWidget>
#include <QObject>
#include <QTreeWidgetItem>
#include <QPoint>
#include "TypeDescFactory.h"
#include "TreeWidget.h"
#include "SerializableWidget.h"


///////////////////////////////////////////////////////////////////////////////

class TreeWidget;
class QPushButton;
class FSTreeNode;
class FSDirNode;
class FSRootNode;
class QTabWidget;
class QListWidget;
class QListWidgetItem;
class QSettings;
class QLineEdit;

///////////////////////////////////////////////////////////////////////////////

class ResourcesBrowser : public QDockWidget, 
                         public FilesystemListener,
                         public FilesystemScanner,
                         public TreeWidgetDescFactory,
                         public SerializableWidget
{
   Q_OBJECT

private:
   enum TaIndex
   {
      TI_Files       = 0,
      TI_Bookmarks   = 1,
   };

private:
   QTabWidget*                   m_tabsManager;
   TreeWidget*                   m_fsTree;
   FSTreeNode*                   m_rootDir;

   // file type toggle action
   QAction*                      m_toggleFileTypesViewBtn;
   bool                          m_viewResourcesOnly;

   // bookmarks
   QListWidget*                  m_bookmarks;

   // find file action
   QAction*                      m_findFile;
   QLineEdit*                    m_searchedFileName;
   QListWidget*                  m_searchResults;

   // resource management stuff
   ResourcesManager*             m_rm;
   QString                       m_iconsDir;
   TypeDescFactory< Resource >*  m_itemsFactory;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   ResourcesBrowser( QWidget* parentWidget );
   ~ResourcesBrowser();

   /**
    * Creates a new resource of the specified type and puts it in the
    * specified parent directory.
    *
    * The browser will ask to specify the resource name etc.
    *
    * @param type
    * @param parentDir
    */
   void createResource( const SerializableReflectionType& type, const std::string& parentDir );

   /**
    * Opens a resource for edition.
    *
    * @param path          path to a resource
    * @param resourceIcon
    */
   void editResource( const std::string& path, const QIcon& resourceIcon );

   /**
    * Adds a new bookmark.
    *
    * @param relativePath
    */
   void addBookmark( const FilePath& relativePath );

   // -------------------------------------------------------------------------
   // SerializableWidget implementation
   // -------------------------------------------------------------------------
   void saveLayout( QSettings& settings );
   void loadLayout( QSettings& settings );

   // -------------------------------------------------------------------------
   // FilesystemListener implementation
   // -------------------------------------------------------------------------
   void onDirChanged( const FilePath& dir );
   void onFileEdited( const FilePath& path );
   void onFileRemoved( const FilePath& path );

   // -------------------------------------------------------------------------
   // FilesystemScanner implementation
   // -------------------------------------------------------------------------
   void onDirectory( const FilePath& name );
   void onFile( const FilePath& name );

   // -------------------------------------------------------------------------
   // TreeWidgetDescFactory implementation
   // -------------------------------------------------------------------------
   unsigned int typesCount() const;
   void getDesc( unsigned int idx, QString& outDesc, QIcon& outIcon ) const;

public slots:
   void onEditResource( QTreeWidgetItem* item, int column );
   void onToggleFilesFiltering();
   void onGetItemsFactory( QTreeWidgetItem* parent, TreeWidgetDescFactory*& outFactoryPtr );
   void onAddNode( QTreeWidgetItem* parent, unsigned int typeIdx );
   void onRemoveNode( QTreeWidgetItem* parent, QTreeWidgetItem* child );
   void onClearNode( QTreeWidgetItem* node );
   void onPopupMenuShown( QTreeWidgetItem* node, QMenu& menu );
   void onFocusOnFile( QListWidgetItem* item );
   void onFindFile();
   void showBookmarksPopupMenu( const QPoint& pt );

private:
   void initUI();
   void refresh( const std::string& rootDir = "/" );

   /**
    * Finds an existing node, providing that it's already been mapped.
    *
    * @param dir
    */
   FSTreeNode* find( const std::string& dir );

   /**
    * Opens a node ( whether it's already been mapped or not ), providing that it exists.
    * Otherwise it will finish at the last valid path element.
    *
    * @param path
    */
   FSTreeNode* open( const FilePath& path );

   /**
    * Focuses the browser on the specified tree node.
    *
    * @param node
    */
   void focusOn( FSTreeNode* node );

   // -------------------------------------------------------------------------
   // interface for the nodes
   // -------------------------------------------------------------------------
   
   friend class FSDirNode;
   friend class FSRootNode;
   inline TreeWidgetDescFactory* getDescFactory() { return this; }
   void addNode( unsigned int idx, const std::string& parentDir );

   // -------------------------------------------------------------------------
   // layout setup helper methods
   // -------------------------------------------------------------------------
   void setupFilesystemTree();
   void setupBookmarks();
   void setupFileFinder();
};

///////////////////////////////////////////////////////////////////////////////

class FSTreeWidget : public TreeWidget
{
   Q_OBJECT

public:
   FSTreeWidget( QWidget* parent, const QString& objName, const QString& iconsDir );

protected:
   QMimeData* mimeData( const QList<QTreeWidgetItem *> items ) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * An action that saves the selected resource.
 */
class SaveResourceAction : public QAction
{
   Q_OBJECT

private:
   Resource&               m_resource;
   bool                    m_recursive;

public:
   /**
    * Constructor.
    *
    * @param icon
    * @param name
    * @param parent
    * @param resource
    * @param recursive     should all dependencies be saved as well?
    */
   SaveResourceAction( const QIcon& icon, const char* name, QObject* parent, Resource& resource, bool recursive );

public slots:
   void onTriggered();
};

///////////////////////////////////////////////////////////////////////////////

class AddBookmarkAction : public QAction
{
   Q_OBJECT

private:
   FilePath                m_relativePath;
   ResourcesBrowser&       m_browser;

public:
   /**
    * Constructor.
    *
    * @param icon
    * @param name
    * @param parent
    * @param relativePath
    * @param browser
    */
   AddBookmarkAction( const QIcon& icon, const char* name, QObject* parent, const FilePath& relativePath, ResourcesBrowser& browser );

public slots:
   void onTriggered();
};

///////////////////////////////////////////////////////////////////////////////

class RemoveBookmarkAction : public QAction
{
   Q_OBJECT

private:
   QListWidget*           m_bookmarksList;
   QListWidgetItem*       m_removedItem;

public:
   /**
    * Constructor.
    *
    * @param icon
    * @param name
    * @param parent
    * @param bookmarksList
    * @param removedItem
    */
   RemoveBookmarkAction( const QIcon& icon, const char* name, QObject* parent, QListWidget* bookmarksList, QListWidgetItem* removedItem );

public slots:
   void onTriggered();
};

///////////////////////////////////////////////////////////////////////////////
