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
#include "TypeDescFactory.h"
#include "TreeWidget.h"


///////////////////////////////////////////////////////////////////////////////

class TreeWidget;
class QPushButton;
class MainAppComponent;
class FSTreeNode;
class FSDirNode;
class FSRootNode;

///////////////////////////////////////////////////////////////////////////////

class ResourcesBrowser : public QObject, 
                         public Component< TamyEditor >,
                         public FilesystemListener,
                         public FilesystemScanner,
                         public GenericFactory< Resource, ResourceEditor >,
                         public TreeWidgetDescFactory
{
   Q_OBJECT

private:
   TamyEditor*                   m_mgr;
   TreeWidget*                   m_fsTree;
   FSTreeNode*                   m_rootDir;
   QPushButton*                  m_toggleFileTypesViewBtn;
   bool                          m_viewResourcesOnly;

   ResourcesManager*             m_rm;
   MainAppComponent*             m_mainApp;
   QString                       m_iconsDir;
   TypeDescFactory< Resource >*  m_itemsFactory;

public:
   /**
    * Constructor.
    */
   ResourcesBrowser();
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
   void createResource( const Class& type, const std::string& parentDir );

   /**
    * Opens a resource for edition.
    *
    * @param path    path to a resource
    */
   void editResource( const std::string& path );

   // -------------------------------------------------------------------------
   // Component initialization
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );

   // -------------------------------------------------------------------------
   // FilesystemListener implementation
   // -------------------------------------------------------------------------
   void onDirChanged( const std::string& dir );

   // -------------------------------------------------------------------------
   // FilesystemScanner implementation
   // -------------------------------------------------------------------------
   void onDirectory( const std::string& name );
   void onFile( const std::string& name );

   // -------------------------------------------------------------------------
   // TreeWidgetDescFactory implementation
   // -------------------------------------------------------------------------
   unsigned int typesCount() const;
   void getDesc( unsigned int idx, QString& outDesc, QIcon& outIcon ) const;

public slots:
   void onEditResource( QTreeWidgetItem* item, int column );
   void onToggleFilesFiltering( bool = false );
   void onGetItemsFactory( QTreeWidgetItem* parent, TreeWidgetDescFactory*& outFactoryPtr );
   void onAddNode( QTreeWidgetItem* parent, unsigned int typeIdx );
   void onRemoveNode( QTreeWidgetItem* parent, QTreeWidgetItem* child );
   void onClearNode( QTreeWidgetItem* node );
   void onPopupMenuShown( QTreeWidgetItem* node, QMenu& menu );

private:
   void initializeEditors();
   void initUI( TamyEditor& mgr );
   void refresh( const std::string& rootDir = "/" );
   FSTreeNode* find( const std::string& dir );

   // -------------------------------------------------------------------------
   // interface for the nodes
   // -------------------------------------------------------------------------
   
   friend class FSDirNode;
   friend class FSRootNode;
   inline TreeWidgetDescFactory* getDescFactory() { return this; }
   void addNode( unsigned int idx, const std::string& parentDir );
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
