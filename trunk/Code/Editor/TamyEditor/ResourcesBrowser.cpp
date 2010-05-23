#include "ResourcesBrowser.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDockWidget>
#include <QPushButton>
#include <QDrag>
#include <QMimeData>
#include <QInputDialog>
#include "MainAppComponent.h"
#include "core.h"
#include "progressdialog.h"
#include "FSNodeMimeData.h"
#include "core-MVC.h"

// nodes
#include "FSTreeNode.h"
#include "FSRootNode.h"
#include "FSDirNode.h"
#include "FSLeafNode.h"

// editors
#include "SceneEditor.h"


///////////////////////////////////////////////////////////////////////////////

ResourcesBrowser::ResourcesBrowser()
: m_mgr( NULL )
, m_fsTree( NULL )
, m_rootDir( NULL )
, m_rm( NULL )
, m_mainApp( NULL )
, m_toggleFileTypesViewBtn( NULL )
, m_viewResourcesOnly( false )
{
}

///////////////////////////////////////////////////////////////////////////////

ResourcesBrowser::~ResourcesBrowser()
{
   delete m_itemsFactory;
   m_itemsFactory = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::initializeEditors()
{
   associate< Model, SceneEditor >();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::initialize( TamyEditor& mgr )
{
   ASSERT( m_mgr == NULL, "ResourcesBrowser component is already initialized" );
   m_mgr = &mgr;

   m_mainApp = &mgr.requestService< MainAppComponent > ();
   m_rm = &mgr.requestService< ResourcesManager >();
   Filesystem& fs = m_rm->getFilesystem();
   fs.attach( *this );

   m_iconsDir = fs.getShortcut( "editorIcons" ).c_str();

   m_itemsFactory = new TypeDescFactory< Resource >( m_iconsDir, fs, "unknownResourceIcon.png" );

   // initialize user interface
   initUI( mgr );
   onToggleFilesFiltering();
   initializeEditors();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::initUI( TamyEditor& mgr )
{
   // setup dockable properties view widget
   QDockWidget* dockWidget = new QDockWidget( "Resources Browser", &mgr );
   dockWidget->setObjectName("ResourcesBrowser/dockWidget");
   mgr.addDockWidget( Qt::LeftDockWidgetArea, dockWidget );

   QWidget* dockWidgetContents = new QWidget( &mgr );
   dockWidgetContents->setObjectName("ResourcesBrowser/dockWidgetContents");
   dockWidget->setWidget( dockWidgetContents );

   QVBoxLayout* layout = new QVBoxLayout( dockWidgetContents );
   dockWidgetContents->setLayout( layout );
   layout->setSpacing(0);
   layout->setMargin(0);

   // toolbar
   QWidget* toolbar = new QWidget( dockWidgetContents );
   dockWidgetContents->setObjectName("ResourcesBrowser/toolbar");
   layout->addWidget( toolbar );

   QHBoxLayout* toolbarLayout = new QHBoxLayout( dockWidgetContents );
   toolbar->setLayout( toolbarLayout );
   toolbarLayout->setSpacing(0);
   toolbarLayout->setMargin(0);

   m_toggleFileTypesViewBtn = new QPushButton( toolbar );
   m_toggleFileTypesViewBtn->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
   toolbarLayout->addWidget( m_toggleFileTypesViewBtn );
   connect( m_toggleFileTypesViewBtn, SIGNAL( clicked( bool ) ), this, SLOT( onToggleFilesFiltering( bool ) ) );

   toolbarLayout->addSpacerItem( new QSpacerItem(40, 1, QSizePolicy::Expanding, QSizePolicy::Fixed) );

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

   m_rootDir = new FSRootNode( m_fsTree, m_rm->getFilesystem() );
   m_fsTree->addTopLevelItem( m_rootDir );
   

   // setup menu entries
   QAction* actionProperties = dockWidget->toggleViewAction();
   actionProperties->setText( QApplication::translate( "TamyEditorClass", "Resources Browser", 0, QApplication::UnicodeUTF8 ) );
   mgr.getViewMenu().addAction( actionProperties );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onDirChanged( const std::string& dir )
{
   refresh( dir );
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

   // if we have a resources manager instance, refill its contents
   if ( !m_rm )
   {
      return;
   }
   const Filesystem& fs = m_rm->getFilesystem();
   fs.scan( rootDir, *this );
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

void ResourcesBrowser::createResource( const Class& type, const std::string& parentDir )
{
   Resource* newResource = type.instantiate< Resource >();

   // learn the new file's name
   const Filesystem& fs = m_rm->getFilesystem();
   std::string extension = newResource->getVirtualExtension();

   bool ok = false;
   QString fullFileName = QInputDialog::getText( m_mgr, "New resource", "Resource name:", QLineEdit::Normal, "", &ok );
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

void ResourcesBrowser::editResource( const std::string& path )
{
   ProgressDialog progressDlg;
   progressDlg.setProgress( 0 );
   Resource& resource = m_rm->create( path );
   progressDlg.setProgress( 1 );

   ResourceEditor* editor = create( resource );
   if ( editor )
   {
      editor->initialize( *m_mgr );
   }
   // CAUTION: we don't manage the editor's lifetime - it has to take care of itself
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onDirectory( const std::string& name )
{
   ASSERT( m_rm != NULL, "This method can only be called when ResourcesManager instance is available" );

   const Filesystem& fs = m_rm->getFilesystem();

   std::string parentDirName = fs.extractDir( name );
   std::string newNodeName = fs.extractNodeName( name );

   FSTreeNode* parent = find( parentDirName );
   ASSERT( parent != NULL, "Parent directory not found" );
   if ( parent )
   { 
      new FSDirNode( parent, newNodeName, fs );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::onFile( const std::string& name )
{
   ASSERT( m_rm != NULL, "This method can only be called when ResourcesManager instance is available" );

   const Filesystem& fs = m_rm->getFilesystem();

   if ( m_viewResourcesOnly )
   {
      std::string extension = fs.extractExtension( name );
      if ( !Resource::isResource( extension ) )
      {
         return;
      }
   }

   std::string parentDirName = fs.extractDir( name );
   std::string newNodeName = fs.extractNodeName( name );

   FSTreeNode* parent = find( parentDirName );
   ASSERT( parent != NULL, "Parent directory not found" );
   if ( parent )
   {  
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

void ResourcesBrowser::onToggleFilesFiltering( bool )
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
      QString newDirName = QInputDialog::getText( m_mgr, "New directory", "Dir name:", QLineEdit::Normal, "", &ok );
      if ( ok )
      {

         fs.mkdir( parentDir + newDirName.toStdString() );
      }
   }
   else
   {
      Class type = m_itemsFactory->getClass( idx - 1 );
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
