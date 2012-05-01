#include "ResourcesBrowser.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDockWidget>
#include <QDrag>
#include <QMimeData>
#include <QInputDialog>
#include <QToolBar>
#include <QMenu>
#include "core.h"
#include "progressdialog.h"
#include "FSNodeMimeData.h"
#include "tamyeditor.h"

// nodes
#include "FSTreeNode.h"
#include "FSRootNode.h"
#include "FSDirNode.h"
#include "FSLeafNode.h"



///////////////////////////////////////////////////////////////////////////////

ResourcesBrowser::ResourcesBrowser( QWidget* parentWidget )
   : QDockWidget( "Resources Browser", parentWidget )
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
   QToolBar* toolbar = new QToolBar( dockWidgetContents );
   dockWidgetContents->setObjectName("ResourcesBrowser/toolbar");
   layout->addWidget( toolbar );

   m_toggleFileTypesViewBtn = new QAction( tr( "Run" ), toolbar );
   toolbar->addAction( m_toggleFileTypesViewBtn );
   connect( m_toggleFileTypesViewBtn, SIGNAL( triggered() ), this, SLOT( onToggleFilesFiltering() ) );

   // setup the scene tree container widget
   m_fsTree = new FSTreeWidget( dockWidgetContents, "ResourcesBrowser/m_fsTree", m_iconsDir );
   layout->addWidget( m_fsTree );

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
   std::vector< std::string > pathParts;
   StringUtils::tokenize( dir, "/", pathParts );
   if ( pathParts.empty() )
   {
      return m_rootDir;
   }

   FSTreeNode* currItem = m_rootDir;
   unsigned int count = pathParts.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      std::string currPathPart = pathParts[i] + "/";
      FSTreeNode* nextItem = currItem->find( currPathPart );
      if ( !nextItem )
      {
         return NULL;
      }

      if ( i + 1 == count )
      {
         return nextItem;
      }

      currItem = nextItem;
   }

   return NULL;
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
      resource = &m_rm->create( path );
   }
   catch( std::exception& ex )
   {
      ASSERT_MSG( false, ex.what() );
   }
   progressDlg.advance();

   if ( resource )
   {
      TamyEditor::getInstance().editResource( *resource, resourceIcon );
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
