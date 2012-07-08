/// @file   TamyEditor/ProjectTree.h
/// @brief  tree that shows a part of the filesystem a project contains
#pragma once

#include <QFrame>
#include <QTreeWidgetItem>
#include "TreeWidget.h"
#include "TreeWidgetDescFactory.h"
#include "core\Filesystem.h"
#include "TypeDescFactory.h"
#include "SerializableWidget.h"


///////////////////////////////////////////////////////////////////////////////

class TreeWidget;
class ProjectTreeNode;
class ProjectDirNode;
class ProjectRootNode;
class QAction;
class Resource;
class QSettings;
class ProjectLeafFactory;

///////////////////////////////////////////////////////////////////////////////

/**
 * Tree that shows a part of the filesystem a project contains.
 */
class ProjectTree : public QFrame, public FilesystemListener, public SerializableWidget
{
   Q_OBJECT

private:
   FilesystemSection*            m_section;

   TreeWidget*                   m_fsTree;
   ProjectTreeNode*              m_rootDir;

   // file type toggle action
   QAction*                      m_toggleFileTypesViewBtn;
   bool                          m_viewResourcesOnly;

   // tools
   QString                       m_iconsDir;
   TreeWidgetDescFactory*        m_itemsFactory;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   ProjectTree( QWidget* parentWidget);
   ~ProjectTree();

   /**
    * Sets the active project section.
    *
    * @param section
    */
   void setSource( FilesystemSection* section );

   /**
    * Finds an existing node, providing that it's already been mapped.
    *
    * @param dir
    */
   ProjectTreeNode* find( const std::string& dir );

   /**
    * Opens a node ( whether it's already been mapped or not ), providing that it exists.
    * Otherwise it will finish at the last valid path element.
    *
    * @param path
    */
   ProjectTreeNode* open( const FilePath& path );

   /**
    * Focuses the browser on the specified tree node.
    *
    * @param node
    */
   void focusOn( ProjectTreeNode* node );

   /**
    * Adds the specified directory ( along with its immediate children, no recursive scan involved ) to the tree.
    *
    * @param dir
    */
   void addDirectory( const FilePath& dir );

   /**
    * Removes a directory and its contents from the tree.
    *
    * @param dir     directory path
    */
   void removeDirectory( const FilePath& dir );

   // -------------------------------------------------------------------------
   // SerializableWidget implementation
   // -------------------------------------------------------------------------
   void saveLayout( QSettings& settings );
   void loadLayout( QSettings& settings );

   // -------------------------------------------------------------------------
   // FilesystemListener implementation
   // -------------------------------------------------------------------------
   void onDirAdded( const FilePath& dir );
   void onDirRemoved( const FilePath& dir );
   void onFileEdited( const FilePath& path );
   void onFileRemoved( const FilePath& path );

public slots:
   void onPopupMenuShown( QTreeWidgetItem* node, QMenu& menu );
   void onGetItemsFactory( QTreeWidgetItem* parent, TreeWidgetDescFactory*& outFactoryPtr );
   void onAddNode( QTreeWidgetItem* parent, unsigned int typeIdx );
   void onRemoveNode( QTreeWidgetItem* parent, QTreeWidgetItem* child );
   void onClearNode( QTreeWidgetItem* node );
   void onItemDoubleClicked( QTreeWidgetItem* item, int column );
   void onToggleFilesFiltering();

signals:
   /**
    * Emitted when we need to populate a pop-up menu specific to the given node.
    *
    * @param node
    * @param menu
    */
   void popupMenuRequest( QTreeWidgetItem* node, QMenu& menu );

private:
   void initialize( const QString& objectName );

      /**
    * Refreshes the contents of the tree, starting from the specified directory.
    *
    * @param rootDir
    */
   void refresh( const std::string& rootDir = "/" );


   // -------------------------------------------------------------------------
   // interface for the nodes
   // -------------------------------------------------------------------------

   friend class ProjectDirNode;
   friend class ProjectRootNode;
   inline TreeWidgetDescFactory* getDescFactory() { return m_itemsFactory; }
   void addNode( unsigned int idx, const std::string& parentDir );
};

///////////////////////////////////////////////////////////////////////////////

class ProjectTreeWidget : public TreeWidget
{
   Q_OBJECT

public:
   ProjectTreeWidget( QWidget* parent, const QString& objName, const QString& iconsDir );

protected:
   QMimeData* mimeData( const QList<QTreeWidgetItem *> items ) const;
};

///////////////////////////////////////////////////////////////////////////////
