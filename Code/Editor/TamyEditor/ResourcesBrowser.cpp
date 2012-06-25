#include "ResourcesBrowser.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDockWidget>
#include <QDrag>
#include <QMimeData>
#include <QInputDialog>
#include <QToolBar>
#include <QMenu>
#include <QTabWidget>
#include <QListWidget>
#include <QSettings>
#include "core.h"
#include "progressdialog.h"
#include "FSNodeMimeData.h"
#include "tamyeditor.h"
#include <list>

// nodes
#include "FSTreeNode.h"
#include "FSRootNode.h"
#include "FSDirNode.h"
#include "FSLeafNode.h"



///////////////////////////////////////////////////////////////////////////////

#define BOOKMARK_FILEPATH_DATA_INDEX      1

///////////////////////////////////////////////////////////////////////////////

ResourcesBrowser::ResourcesBrowser( QWidget* parentWidget )
   : QDockWidget( "Resources Browser", parentWidget )
   , m_tabsManager( NULL )
   , m_fsTree( NULL )
   , m_rootDir( NULL )
   , m_rm( NULL )
   , m_toggleFileTypesViewBtn( NULL )
   , m_viewResourcesOnly( false )
{
   setObjectName("ResourcesBrowser/dockWidget");

   m_rm = &ResourcesManager::getInstance();
   Filesystem& fs = m_rm->getFilesystem();
   fs.attach( *this );

   m_iconsDir = fs.getShortcut( "editorIcons" ).c_str();

   m_itemsFactory = new TypeDescFactory< Resource >( m_iconsDir, fs, "unknownResourceIcon.png" );

   // initialize user interface
   initUI();
   onToggleFilesFiltering();
}

///////////////////////////////////////////////////////////////////////////////

ResourcesBrowser::~ResourcesBrowser()
{
   delete m_itemsFactory;
   m_itemsFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::initUI()
{
   // setup dockable properties view widget
   QWidget* dockWidgetContents = new QWidget( this );
   dockWidgetContents->setObjectName("ResourcesBrowser/dockWidgetContents");
   setWidget( dockWidgetContents );

   QVBoxLayout* layout = new QVBoxLayout( dockWidgetContents );
   dockWidgetContents->setLayout( layout );
   layout->setSpacing(0);
   layout->setMargin(0);

   // toolbar
   {
      QToolBar* toolbar = new QToolBar( dockWidgetContents );
      dockWidgetContents->setObjectName("ResourcesBrowser/toolbar");
      layout->addWidget( toolbar );

      // toggle file tabs
      {
         m_toggleFileTypesViewBtn = new QAction( tr( "Toggle file types" ), toolbar );
         toolbar->addAction( m_toggleFileTypesViewBtn );
         connect( m_toggleFileTypesViewBtn, SIGNAL( triggered() ), this, SLOT( onToggleFilesFiltering() ) );
      }

      // find file
      {
         toolbar->addSeparator();

         m_findFile = new QAction( tr( "Find file" ), toolbar );
         m_findFile->setIcon( QIcon( m_iconsDir + "search.png" ) );
         toolbar->addAction( m_findFile );
         connect( m_findFile, SIGNAL( triggered() ), this, SLOT( onFindFile() ) );

         m_searchedFileName = new QLineEdit( toolbar );
         toolbar->addWidget( m_searchedFileName );
      }

   }

   // create the tabs manager that will host the tabs with the filesystem tree and the list of bookmarks
   {
      m_tabsManager = new QTabWidget( dockWidgetContents );
      m_tabsManager->setObjectName( "ResourcesBrowser/tabsManager" );
      layout->addWidget( m_tabsManager );

      // setup the scene tree container widget
      {
         m_fsTree = new FSTreeWidget( m_tabsManager, "ResourcesBrowser/m_fsTree", m_iconsDir );
         m_tabsManager->addTab( m_fsTree, "Files" );

         QStringList columnLabels; 
         columnLabels.push_back( "Name" );
         columnLabels.push_back( "Size" );
         m_fsTree->setColumnCount( columnLabels.size() );
         m_fsTree->setHeaderLabels( columnLabels );
         m_fsTree->setDragEnabled( true ); 
         m_fsTree->setDropIndicatorShown( true ); 
         connect( m_fsTree, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( onEditResource( QTreeWidgetItem*, int ) ) );
         connect( m_fsTree, SIGNAL( getItemsFactory( QTreeWidgetItem*, TreeWidgetDescFactory*& ) ), this, SLOT( onGetItemsFactory( QTreeWidgetItem*, TreeWidgetDescFactory*& ) ) );
         connect( m_fsTree, SIGNAL( addNode( QTreeWidgetItem*, unsigned int ) ), this, SLOT( onAddNode( QTreeWidgetItem*, unsigned int ) ) );
         connect( m_fsTree, SIGNAL( removeNode( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( onRemoveNode( QTreeWidgetItem*, QTreeWidgetItem* ) ) );
         connect( m_fsTree, SIGNAL( clearNode( QTreeWidgetItem* ) ), this, SLOT( onClearNode( QTreeWidgetItem* ) ) );
         connect( m_fsTree, SIGNAL( popupMenuShown( QTreeWidgetItem*, QMenu& ) ), this, SLOT( onPopupMenuShown( QTreeWidgetItem*, QMenu& ) ) );

         m_rootDir = new FSRootNode( m_fsTree, m_rm->getFilesystem() );
         m_fsTree->addTopLevelItem( m_rootDir );

         m_rootDir->setExpanded( true );
      }

      // setup the bookmarks tab
      {
         m_bookmarks = new QListWidget( m_tabsManager );
         m_tabsManager->addTab( m_bookmarks, "Bookmarks" );

         connect( m_bookmarks, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), this, SLOT( onJumpToBookmark( QListWidgetItem* ) ) );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onDirChanged( const FilePath& dir )
{
   refresh( dir.getRelativePath() );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onFileEdited( const FilePath& path )
{
   FilePath dir;
   path.extractDir( dir );
   refresh( dir.getRelativePath() );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onFileRemoved( const FilePath& path )
{
   FilePath dir;
   path.extractDir( dir );
   refresh( dir.getRelativePath() );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::refresh( const std::string& rootDir )
{
   // find the entry corresponding to the specified root directory
   FSTreeNode* entry = find( rootDir );
   if ( !entry )
   {
      return;
   }

   // clean it up
   entry->clear();

   // scan the contents of the resources manager
   if ( m_rm != NULL )
   {
      m_rm->scan( rootDir, *this, false );
   }
}

///////////////////////////////////////////////////////////////////////////////

FSTreeNode* ResourcesBrowser::find( const std::string& dir )
{
   // split the path int particular elements
   FilePath path( dir );
   std::vector< std::string > pathParts;
   path.getElements( pathParts );

   // if there were no elements, simply return the root node
   if ( pathParts.empty() )
   {
      return m_rootDir;
   }

   // find the node
   FSTreeNode* currItem = m_rootDir;
   unsigned int count = pathParts.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      std::string currPathPart = pathParts[i] + "/";
      FSTreeNode* nextItem = currItem->find( currPathPart );
      if ( !nextItem )
      {
         // that's the last leaf - there's nothing else to search in - bail
         break;
      }

      if ( i + 1 == count )
      {
         // we found it
         return nextItem;
      }

      // keep on looking starting from the node we've just analyzed
      currItem = nextItem;
   }

   // the node hasn't been mapped yet
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

FSTreeNode* ResourcesBrowser::open( const std::string& dir )
{
   // split the path int particular elements
   FilePath path( dir );
   std::vector< std::string > pathParts;
   path.getElements( pathParts );

   // if there were no elements, simply return the root node
   if ( pathParts.empty() )
   {
      return m_rootDir;
   }

   // start mapping the nodes
   std::string currRelativePath = "/";

   FSTreeNode* currItem = m_rootDir;
   unsigned int count = pathParts.size();

   FSTreeNode* foundItem = NULL;
   for ( unsigned int i = 0; i < count; ++i )
   {
      std::string currPathPart = pathParts[i] + "/";
      FSTreeNode* nextItem = currItem->find( currPathPart );
      if ( !nextItem )
      {
         // that's the last leaf thus far - so scan it
         m_rm->scan( currRelativePath, *this, false );

         // and try finding the corresponding node again
         nextItem = currItem->find( currPathPart );
      }

      if ( !nextItem )
      {
         // still nothing - this means that the path we're looking for doesn't exist - so bail
         break;
      }

      if ( i + 1 == count )
      {
         // we found it
         foundItem = nextItem;
         break;
      }

      // keep on looking starting from the node we've just analyzed
      currRelativePath += currPathPart;
      currItem = nextItem;
   }

   if ( foundItem )
   {
      // we found it - just to be sure, rescan the contents
      m_rm->scan( foundItem->getRelativePath(), *this, false );

      if ( dynamic_cast< FSDirNode* >( foundItem ) )
      {
         foundItem->setExpanded( true );
      }
   }

   // the path doesn't exist
   return foundItem;
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::focusOn( FSTreeNode* node )
{
   m_fsTree->setCurrentItem( node );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::createResource( const SerializableReflectionType& type, const std::string& parentDir )
{
   Resource* newResource = type.instantiate< Resource >();

   // learn the new file's name
   const Filesystem& fs = m_rm->getFilesystem();
   std::string extension = newResource->getVirtualExtension();

   bool ok = false;
   QString fullFileName = QInputDialog::getText( this, "New resource", "Resource name:", QLineEdit::Normal, "", &ok );
   if ( !ok ) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }

   // once the file is open, extract the directory name
   std::string fileName = parentDir + fullFileName.toStdString() + "." + extension;

   // create & save the resource
   newResource->setFilePath( fileName );
   m_rm->addResource( newResource );
   newResource->saveResource();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::editResource( const std::string& path, const QIcon& resourceIcon )
{
   ProgressDialog progressDlg;
   progressDlg.initialize( "Loading a resource", 1 );
   Resource* resource = NULL;

   try
   {
      resource = m_rm->create( path );
   }
   catch( std::exception& ex )
   {
      ASSERT_MSG( false, ex.what() );
   }
   progressDlg.advance();

   if ( resource )
   {
      TamyEditor& tamyEd = TamyEditor::getInstance();
      if ( tamyEd.activateResourceEditor( resource ) == false )
      {
         ResourceEditor* resourceEd = tamyEd.createResourceEditor( resource, resourceIcon );
         tamyEd.addResourceEditor( resourceEd );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onDirectory( const FilePath& name )
{
   ASSERT_MSG( m_rm != NULL, "This method can only be called when ResourcesManager instance is available" );

   const Filesystem& fs = m_rm->getFilesystem();

   std::string parentDirName = fs.extractDir( name );
   std::string newNodeName = fs.extractNodeName( name );

   FSTreeNode* parent = find( parentDirName );
   ASSERT_MSG( parent != NULL, "Parent directory not found" );
   if ( parent && parent->find( newNodeName ) == NULL )
   { 
      // add the new node ( but only if it's unique )
      new FSDirNode( parent, newNodeName, fs );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onFile( const FilePath& name )
{
   ASSERT_MSG( m_rm != NULL, "This method can only be called when ResourcesManager instance is available" );

   const Filesystem& fs = m_rm->getFilesystem();

   if ( m_viewResourcesOnly )
   {
      std::string extension = name.extractExtension();
      if ( !Resource::isResource( extension ) )
      {
         return;
      }
   }

   FilePath parentDirName;
   name.extractDir( parentDirName );
   std::string newNodeName = name.extractNodeName();

   FSTreeNode* parent = find( parentDirName.getRelativePath() );
   ASSERT_MSG( parent != NULL, "Parent directory not found" );
   if ( parent && parent->find( newNodeName ) == NULL )
   {
      // add the new node ( but only if it's unique )
      new FSLeafNode( parent, newNodeName, fs, *m_itemsFactory );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onEditResource( QTreeWidgetItem* item, int column )
{
   FSTreeNode* entry = dynamic_cast< FSTreeNode* >( item );
   entry->editResource( *this );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onToggleFilesFiltering()
{
   // change the view mode flag
   m_viewResourcesOnly = !m_viewResourcesOnly;

   // update the icon on the view type toggling button
   QString iconName;
   if ( m_viewResourcesOnly )
   {
      iconName = m_iconsDir + "resource.png";
      m_toggleFileTypesViewBtn->setToolTip( "Switch to viewing all files" );
   }
   else
   {
      iconName = m_iconsDir + "fileInDir.png";
      m_toggleFileTypesViewBtn->setToolTip( "Switch to viewing resources only" );
   }
   m_toggleFileTypesViewBtn->setIcon( QIcon( iconName ) );

   // refresh the view
   m_rootDir->clear();
   refresh();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onGetItemsFactory( QTreeWidgetItem* parent, TreeWidgetDescFactory*& outFactoryPtr )
{
   FSTreeNode* item = dynamic_cast< FSTreeNode* >( parent );
   outFactoryPtr = item->getDescFactory( *this );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onAddNode( QTreeWidgetItem* parent, unsigned int typeIdx )
{  
   FSTreeNode* item = dynamic_cast< FSTreeNode* >( parent );
   item->addNode( typeIdx, *this );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onRemoveNode( QTreeWidgetItem* parent, QTreeWidgetItem* child )
{
   FSTreeNode* parentItem = dynamic_cast< FSTreeNode* >( parent );
   FSTreeNode* childItem = dynamic_cast< FSTreeNode* >( child );
   parentItem->removeNode( childItem );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onClearNode( QTreeWidgetItem* node )
{
   FSTreeNode* item = dynamic_cast< FSTreeNode* >( node );
   item->clearNodes();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onPopupMenuShown( QTreeWidgetItem* node, QMenu& menu )
{
   // create additional save actions, providing the item we clicked
   // is a loaded resource
   FSLeafNode* nodeItem = dynamic_cast< FSLeafNode* >( node );
   if ( nodeItem )
   {
      Resource* resource = m_rm->findResource( nodeItem->getRelativePath() );
      if ( resource != NULL )
      {
         menu.addSeparator();

         QAction* saveNodeAction = new SaveResourceAction( QIcon( m_iconsDir + "saveFile.png" ), "Save", this, *resource, false );
         menu.addAction( saveNodeAction );

         QAction* saveHierarchyAction = new SaveResourceAction( QIcon( m_iconsDir + "saveFile.png" ), "Save All", this, *resource, true );
         menu.addAction( saveHierarchyAction );
      }
   }

   // create additional save actions providing the item we clicked
   // is a directory 
   FSDirNode* dirNode = dynamic_cast< FSDirNode* >( node );
   if ( dirNode )
   {
      menu.addSeparator();

      QAction* addBookmarkAction = new AddBookmarkAction( QIcon( m_iconsDir + "addBookmark.png" ), "Add bookmark", this, FilePath( dirNode->getRelativePath() ), *this );
      menu.addAction( addBookmarkAction );
   }
}

///////////////////////////////////////////////////////////////////////////////

unsigned int ResourcesBrowser::typesCount() const
{
   return m_itemsFactory->typesCount() + 1;
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::getDesc( unsigned int idx, QString& outDesc, QIcon& outIcon ) const
{
   if ( idx == 0 )
   {
      // a directory
      outDesc = "Directory";
      outIcon = QIcon( m_iconsDir + "dirIcon.png" );
   }
   else
   {
      // a resource
      m_itemsFactory->getDesc( idx - 1, outDesc, outIcon );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::addNode( unsigned int idx, const std::string& parentDir )
{
   if ( idx == 0 )
   {
      // learn the new file's name
      const Filesystem& fs = m_rm->getFilesystem();
      bool ok = false;
      QString newDirName = QInputDialog::getText( this, "New directory", "Dir name:", QLineEdit::Normal, "", &ok );
      if ( ok )
      {

         fs.mkdir( parentDir + newDirName.toStdString() );
      }
   }
   else
   {
      const SerializableReflectionType& type = m_itemsFactory->getClass( idx - 1 );
      createResource( type, parentDir );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::addBookmark( const FilePath& relativePath )
{
   QListWidgetItem* item = new QListWidgetItem();

   std::string nodeName = relativePath.extractNodeName();
   item->setText( nodeName.c_str() );
   item->setData( BOOKMARK_FILEPATH_DATA_INDEX, relativePath.c_str() );

   m_bookmarks->addItem( item );

   // activate the bookmarks tab
   m_tabsManager->setCurrentIndex( TI_Bookmarks );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onJumpToBookmark( QListWidgetItem* item )
{
   std::string path = item->data( BOOKMARK_FILEPATH_DATA_INDEX ).toString().toStdString();
   FSTreeNode* node = open( path );
   if ( !node )
   {
      // the node doesn't exist - do nothing
      return;
   }

   // focus on that item
   focusOn( node );

   // activate the files tab
   m_tabsManager->setCurrentIndex( TI_Files );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onFindFile()
{
   std::string fileName = m_searchedFileName->text().toStdString();
   
   std::vector< FilePath > paths;
   m_rm->getFilesystem().find( fileName, paths );
   if ( !paths.empty() )
   {
      open( paths[0] );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::saveLayout( QSettings& settings )
{
   // begin serialization
   settings.beginGroup( "ResourcesBrowser/layoutSettings" );

   // serialize the list of bookmarks
   {
      QString bookmarks;

      uint bookmarksCount = m_bookmarks->count();
      for ( uint i = 0; i < bookmarksCount; ++i )
      {
          bookmarks += ";" + m_bookmarks->item( i )->data( BOOKMARK_FILEPATH_DATA_INDEX ).toString();
      }

      settings.setValue( "bookmarks", bookmarks );
   }

   // save all open directories
   {
      QString openDirectories;
      
      std::list< FSTreeNode* > nodesToExplore;
      nodesToExplore.push_back( m_rootDir );
      while ( !nodesToExplore.empty() )
      {
         FSTreeNode* currNode = nodesToExplore.front(); nodesToExplore.pop_front();
         if ( currNode->isExpanded() )
         {
            // memorize only expanded nodes
            openDirectories += ";" + QString( currNode->getRelativePath().c_str() );
         }

         uint childrenCount = currNode->childCount();
         for ( uint i = 0; i < childrenCount; ++i )
         {
            FSDirNode* childDirNode = dynamic_cast< FSDirNode* >( currNode->child( i ) );
            if ( childDirNode )
            {
               nodesToExplore.push_back( childDirNode );
            }
         }
      }

      settings.setValue( "openDirectories", openDirectories );
   }

   // finish serialization
   settings.endGroup();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::loadLayout( QSettings& settings )
{
   // begin serialization
   settings.beginGroup( "ResourcesBrowser/layoutSettings" );

   // deserialize the list of bookmarks
   {
      std::string bookmarks = settings.value( "bookmarks" ).toString().toStdString();
      std::vector< std::string > arrBookmarks;
      StringUtils::tokenize( bookmarks, ";", arrBookmarks );

      uint count = arrBookmarks.size();
      for ( uint i = 0; i < count; ++i )
      {
         addBookmark( FilePath( arrBookmarks[i] ) );
      }
   }

   // deserialize all open directories
   {
      std::string openDirectories = settings.value( "openDirectories" ).toString().toStdString();

      std::vector< std::string > arrOpenDirectories;
      StringUtils::tokenize( openDirectories, ";", arrOpenDirectories );
      uint count = arrOpenDirectories.size();
      for ( uint i = 0; i < count; ++i )
      {
         open( arrOpenDirectories[i] );
      }
   }

   // finish serialization
   settings.endGroup();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

FSTreeWidget::FSTreeWidget( QWidget* parent, const QString& objName, const QString& iconsDir )
   : TreeWidget( parent, objName, iconsDir )
{
}

///////////////////////////////////////////////////////////////////////////////

QMimeData* FSTreeWidget::mimeData( const QList<QTreeWidgetItem *> items ) const
{
   QMimeData* data = __super::mimeData( items );

   std::vector< std::string > paths;
   unsigned int count = items.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      FSTreeNode* node = dynamic_cast< FSTreeNode* >( items[ i ] );
      if ( node )
      {
         paths.push_back( node->getRelativePath() );
      }
   }

   FSNodeMimeData dataEncoder( paths );
   dataEncoder.save( *data );

   return data;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SaveResourceAction::SaveResourceAction( const QIcon& icon, const char* name, QObject* parent, Resource& resource, bool recursive )
   : QAction( icon, name, parent )
   , m_resource( resource )
   , m_recursive( recursive )
{
   connect( this, SIGNAL( triggered() ), this, SLOT( onTriggered() ) );
}

///////////////////////////////////////////////////////////////////////////////

void SaveResourceAction::onTriggered()
{
   m_resource.saveResource();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

AddBookmarkAction::AddBookmarkAction( const QIcon& icon, const char* name, QObject* parent, const FilePath& relativePath, ResourcesBrowser& browser )
   : QAction( icon, name, parent )
   , m_relativePath( relativePath )
   , m_browser( browser )
{
   connect( this, SIGNAL( triggered() ), this, SLOT( onTriggered() ) );
}

///////////////////////////////////////////////////////////////////////////////

void AddBookmarkAction::onTriggered()
{
   m_browser.addBookmark( m_relativePath );
}

///////////////////////////////////////////////////////////////////////////////
