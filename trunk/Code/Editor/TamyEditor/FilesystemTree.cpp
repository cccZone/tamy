#include "FilesystemTree.h"
#include "core.h"

// widgets
#include <QVBoxLayout>
#include <QToolbar>
#include <QInputDialog>

// tree widget
#include "FSNodeMimeData.h"
#include <QTreeWidgetItem>

// nodes
#include "FSTreeNode.h"
#include "FSRootNode.h"
#include "FSDirNode.h"
#include "FSLeafNode.h"
#include "FSTreeItemsFactory.h"

// tools
#include "ResourceManagementUtil.h"
#include <QSettings>
#include "Project.h"
#include "AdditionalTreeItemFactories.h"


///////////////////////////////////////////////////////////////////////////////

FilesystemTree::FilesystemTree( QWidget* parentWidget )
   : QFrame( parentWidget )
   , m_fsTree( NULL )
   , m_rootDir( NULL )
   , m_toggleFileTypesViewBtn( NULL )
   , m_viewResourcesOnly( false )
   , m_itemsFactory( NULL )
{
   // give the frame a meaningful name so that it's taken into consideration during UI serialization
   QString objectName = "ResourcesBrowser/filesystemTreeFrame";
   setObjectName( objectName );

   // setup the frame layout
   QVBoxLayout* layout = new QVBoxLayout( this );
   layout->setSpacing(0);
   layout->setMargin(0);
   setLayout( layout );

   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   m_iconsDir = fs.getShortcut( "editorIcons" ).c_str();

   // attach self to the filesystem to keep an eye on any changes happening to it
   fs.attach( *this );

   // setup the items factory
   m_itemsFactory = new FSTreeItemsFactory( m_iconsDir );

   // setup the widgets
   initialize( objectName );
   onToggleFilesFiltering();
}

///////////////////////////////////////////////////////////////////////////////

FilesystemTree::~FilesystemTree()
{
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   fs.detach( *this );

   delete m_itemsFactory;
   m_itemsFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::initialize( const QString& topObjectName )
{
   // toolbar
   {
      QToolBar* toolbar = new QToolBar( this );
      toolbar->setObjectName( topObjectName + "/toolbar" );
      layout()->addWidget( toolbar );

      // toggle file tabs
      {
         m_toggleFileTypesViewBtn = new QAction( tr( "Toggle file types" ), toolbar );
         toolbar->addAction( m_toggleFileTypesViewBtn );
         connect( m_toggleFileTypesViewBtn, SIGNAL( triggered() ), this, SLOT( onToggleFilesFiltering() ) );
      }
   }

   // filesystem tree
   {
      m_fsTree = new FSTreeWidget( this, topObjectName + "/m_fsTree", m_iconsDir );
      layout()->addWidget( m_fsTree );

      QStringList columnLabels; 
      columnLabels.push_back( "Name" );
      columnLabels.push_back( "Size" );
      m_fsTree->setColumnCount( columnLabels.size() );
      m_fsTree->setHeaderLabels( columnLabels );
      m_fsTree->setDragEnabled( true ); 
      m_fsTree->setDropIndicatorShown( true ); 
      connect( m_fsTree, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( onItemDoubleClicked( QTreeWidgetItem*, int ) ) );
      connect( m_fsTree, SIGNAL( getItemsFactory( QTreeWidgetItem*, TreeWidgetDescFactory*& ) ), this, SLOT( onGetItemsFactory( QTreeWidgetItem*, TreeWidgetDescFactory*& ) ) );
      connect( m_fsTree, SIGNAL( addNode( QTreeWidgetItem*, unsigned int ) ), this, SLOT( onAddNode( QTreeWidgetItem*, unsigned int ) ) );
      connect( m_fsTree, SIGNAL( removeNode( QTreeWidgetItem*, QTreeWidgetItem* ) ), this, SLOT( onRemoveNode( QTreeWidgetItem*, QTreeWidgetItem* ) ) );
      connect( m_fsTree, SIGNAL( clearNode( QTreeWidgetItem* ) ), this, SLOT( onClearNode( QTreeWidgetItem* ) ) );
      connect( m_fsTree, SIGNAL( popupMenuShown( QTreeWidgetItem*, QMenu& ) ), this, SLOT( onPopupMenuShown( QTreeWidgetItem*, QMenu& ) ) );

      ResourcesManager& rm = ResourcesManager::getInstance();

      m_rootDir = new FSRootNode( m_fsTree );
      m_fsTree->addTopLevelItem( m_rootDir );

      m_rootDir->setExpanded( true );
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onToggleFilesFiltering()
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
   refreshRecursive();
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onDirAdded( const FilePath& dir )
{
   addDirectory( dir );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onDirRemoved( const FilePath& dir )
{
   removeDirectory( dir );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onFileEdited( const FilePath& path )
{
   // do nothing
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onFileRemoved( const FilePath& path )
{
   FilePath dir;
   path.extractDir( dir );
   refresh( dir.getRelativePath() );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::addDirectory( const FilePath& dir )
{
   // add the path to it - piece by piece
   std::vector< std::string > pathElements;
   StringUtils::tokenize( dir.c_str(), "/\\", pathElements );
   uint count = pathElements.size();

   FSTreeNode* currNode = m_rootDir;
   for ( uint i = 0; i < count; ++i )
   {
      FSTreeNode* childNode = currNode->find( pathElements[i] );
      if ( childNode )
      {
         // the node exists - follow it deeper
         currNode = childNode;
      }
      else
      {
         // the node doesn't exist - create a new one
         currNode = new FSDirNode( currNode, pathElements[i] + "/" );
      }
   }

   // once the path's been recreated, scan the final directory
   refresh( dir );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::removeDirectory( const FilePath& dir )
{
   // find a node corresponding to the specified path
   FSTreeNode* node = find( dir );
   if ( !node )
   {
      return;
   }

   QTreeWidgetItem* parentItem = node->parent();
   if ( parentItem )
   {
      int childIdx = parentItem->indexOfChild( node );
      parentItem->takeChild( childIdx );
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::refreshRecursive( const std::string& rootDir ) 
{
   FSTreeNode* rootDirNode = find( rootDir );
   if ( !rootDirNode )
   {
      return;
   }

   // collect all directories that need refreshing
   std::vector< std::string > directoriesToRefresh;
   {
      Stack< FSTreeNode* > dirs;
      dirs.push( rootDirNode );
      while ( !dirs.empty() )
      {
         FSTreeNode* analyzedNode = dirs.pop();
         if ( dynamic_cast< FSLeafNode* >( analyzedNode ) != NULL )
         {
            // this is a leaf node - we only want to scan composite nodes
            continue;
         }
         directoriesToRefresh.push_back( analyzedNode->getRelativePath() );

         int childrenCount = analyzedNode->childCount();
         for ( int i = 0; i < childrenCount; ++i )
         {
            dirs.push( static_cast< FSTreeNode* >( analyzedNode->child( i ) ) );
         }
      }
   }

   // refresh collected directories
   uint count = directoriesToRefresh.size();
   for ( uint i = 0; i < count; ++i )
   {
      refresh( directoriesToRefresh[i] );
   }
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::refresh( const std::string& rootDir )
{
   // find the entry corresponding to the specified root directory
   FSTreeNode* rootDirNode = find( rootDir );
   if ( !rootDirNode )
   {
      return;
   }

   // collect the items in the root dir, so that we can keep track of what items have disappeared
   int count = rootDirNode->childCount();
   std::set< FSTreeNode* > childrenToRemove;
   for ( int i = 0; i < count; ++i )
   {
      childrenToRemove.insert( static_cast< FSTreeNode* >( rootDirNode->child(i) ) );
   }


   // scan the contents of the resources manager
   struct Scanner : public FilesystemScanner
   {
      FSTreeNode*                      m_rootDirNode;
      TreeWidgetDescFactory*           m_itemsFactory;
      bool                             m_viewResourcesOnly;
      std::set< FSTreeNode* >&    m_childrenToRemove;

      // Even though this scanner is the only reference point of m_itemsFactory,
      // we're gonna leave it defined locally in the method for two reasons:
      //  - it's applicable ONLY in context of this method, and it would break encapsulation
      //    if we allowed for FS scans without going through this method, which actually does a bit more
      //    than just the scan
      //  - we'd still have to pass those two values here, so it would just be a mere param shift one line up in the code:
      //       {  // was like this
      //          FilesystemTree tree( parentWidget, objectName, itemsFactory );
      //       }
      //
      //       {  // would be if I extracted this class
      //          Scanner scanner( itemsFactory );
      //          FilesystemTree tree( parentWidget, objectName, scanner );
      //       }
      //
      //       so this operation would simplify nothing

      Scanner( FSTreeNode* rootDirNode, TreeWidgetDescFactory* itemsFactory, bool viewResourcesOnly, std::set< FSTreeNode* >& childrenToRemove )
         : m_rootDirNode( rootDirNode )
         , m_itemsFactory( itemsFactory )
         , m_viewResourcesOnly( viewResourcesOnly )
         , m_childrenToRemove( childrenToRemove )
      {
      }

      void onDirectory( const FilePath& name )
      {
         const Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
         std::string newNodeName = fs.extractNodeName( name );

         FSTreeNode* node = m_rootDirNode->find( newNodeName );
         if ( node == NULL )
         {
            // add the new node ( but only if it's unique )
            new FSDirNode( m_rootDirNode, newNodeName );
         }
         else
         {
            // already got it in the tree, so remove it from the list of items to remove
            m_childrenToRemove.erase( node );
         }
      }

      void onFile( const FilePath& name )
      {
         if ( m_viewResourcesOnly )
         {
            std::string extension = name.extractExtension();
            if ( !Resource::isResource( extension ) )
            {
               return;
            }
         }

         std::string newNodeName = name.extractNodeName();

         FSTreeNode* node = m_rootDirNode->find( newNodeName );
         if ( node == NULL )
         {
            // add the new node ( but only if it's unique )
            new FSLeafNode( m_rootDirNode, newNodeName, *m_itemsFactory );
         }
         else
         {
            // already got it in the tree, so remove it from the list of items to remove
            m_childrenToRemove.erase( node );
         }
      }
   };

   // scan the directory
   Scanner scanner( rootDirNode, m_itemsFactory, m_viewResourcesOnly, childrenToRemove );
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   resMgr.scan( rootDir, scanner, false );

   // remove the remaining items
   for ( std::set< FSTreeNode* >::iterator it = childrenToRemove.begin(); it != childrenToRemove.end(); ++it )
   {
      FSTreeNode* item = *it;
      rootDirNode->removeChild( item );
   }
}

///////////////////////////////////////////////////////////////////////////////

FSTreeNode* FilesystemTree::find( const std::string& dir )
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
      std::string currPathPart = pathParts[i];
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

FSTreeNode* FilesystemTree::open( const FilePath& path )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   // split the path int particular elements
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
      std::string currPathPart = pathParts[i];
      FSTreeNode* nextItem = currItem->find( currPathPart );
      if ( !nextItem )
      {
         // that's the last leaf thus far - so scan it
         refresh( currRelativePath );

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
      refresh( foundItem->getRelativePath() );

      if ( dynamic_cast< FSDirNode* >( foundItem ) )
      {
         foundItem->setExpanded( true );
      }
   }

   // the path doesn't exist
   return foundItem;
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::focusOn( FSTreeNode* node )
{
   m_fsTree->setCurrentItem( node );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::addNode( unsigned int idx, const std::string& parentDir )
{
   switch( idx )
   {
      case FS_ITEM_DIR:
      {
         // learn the new file's name
         bool ok = false;
         QString newDirName = QInputDialog::getText( this, "New directory", "Dir name:", QLineEdit::Normal, "", &ok );
         if ( ok )
         {
            const Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
            fs.mkdir( parentDir + newDirName.toStdString() );
         }

         break;
      }
   
      case FS_ITEM_PROJECT:
      {
         // learn the new file's name
         bool ok = false;
         QString newProjectName = QInputDialog::getText( this, "New project", "Project name:", QLineEdit::Normal, "", &ok );
         if ( ok )
         {
            // create a project directory
            ResourcesManager& resMgr = ResourcesManager::getInstance();
            const Filesystem& fs = resMgr.getFilesystem();

            FilePath projectDir( parentDir + newProjectName.toStdString() );
            fs.mkdir( projectDir );

            // and place a new project resource there
            std::string projectPath = projectDir.c_str();
            projectPath += "/";
            projectPath += newProjectName.toStdString() + "." + Project::getExtension();
            resMgr.create< Project >( FilePath( projectPath ) );
         }

         break;
      }
   }

}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onGetItemsFactory( QTreeWidgetItem* parent, TreeWidgetDescFactory*& outFactoryPtr )
{
   FSTreeNode* item = dynamic_cast< FSTreeNode* >( parent );
   outFactoryPtr = item->getDescFactory( *this );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onAddNode( QTreeWidgetItem* parent, unsigned int typeIdx )
{  
   FSTreeNode* item = dynamic_cast< FSTreeNode* >( parent );
   item->addNode( typeIdx, *this );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onRemoveNode( QTreeWidgetItem* parent, QTreeWidgetItem* child )
{
   FSTreeNode* parentItem = dynamic_cast< FSTreeNode* >( parent );
   FSTreeNode* childItem = dynamic_cast< FSTreeNode* >( child );
   parentItem->removeNode( childItem );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onClearNode( QTreeWidgetItem* node )
{
   FSTreeNode* item = dynamic_cast< FSTreeNode* >( node );
   item->clearNodes();
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onItemDoubleClicked( QTreeWidgetItem* item, int column )
{
   FSTreeNode* entry = dynamic_cast< FSTreeNode* >( item );
   entry->openItem( *this );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::onPopupMenuShown( QTreeWidgetItem* node, QMenu& menu )
{
   emit popupMenuRequest( node, menu );
}

///////////////////////////////////////////////////////////////////////////////

void FilesystemTree::saveLayout( QSettings& settings )
{
   // begin serialization
   settings.beginGroup( objectName() + "/layoutSettings" );

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

void FilesystemTree::loadLayout( QSettings& settings )
{
   // begin serialization
   settings.beginGroup( objectName() + "/layoutSettings" );

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
