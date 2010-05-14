#include "ResourcesBrowser.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDockWidget>
#include <QPushButton>
#include <QDrag>
#include <QMimeData>
#include "MainAppComponent.h"
#include "core.h"
#include "progressdialog.h"

// editors
#include "core-MVC.h"
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
   m_rm->getFilesystem().attach( *this );

   // initialize user interface
   initUI( mgr );
   toggleFilesFiltering();
   initializeEditors();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::initUI( TamyEditor& mgr )
{
   // setup dockable properties view widget
   QDockWidget* dockWidget = new QDockWidget( "Resources Browser", &mgr );
   mgr.addDockWidget( Qt::LeftDockWidgetArea, dockWidget );

   QWidget* dockWidgetContents = new QWidget( &mgr );
   dockWidget->setWidget( dockWidgetContents );

   QVBoxLayout* layout = new QVBoxLayout( dockWidgetContents );
   dockWidgetContents->setLayout( layout );
   layout->setSpacing(0);
   layout->setMargin(0);

   // toolbar
   QWidget* toolbar = new QWidget( dockWidgetContents );
   layout->addWidget( toolbar );

   QHBoxLayout* toolbarLayout = new QHBoxLayout( dockWidgetContents );
   toolbar->setLayout( toolbarLayout );
   toolbarLayout->setSpacing(0);
   toolbarLayout->setMargin(0);

   m_toggleFileTypesViewBtn = new QPushButton( toolbar );
   m_toggleFileTypesViewBtn->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
   toolbarLayout->addWidget( m_toggleFileTypesViewBtn );
   connect( m_toggleFileTypesViewBtn, SIGNAL( clicked( bool ) ), this, SLOT( toggleFilesFiltering( bool ) ) );

   toolbarLayout->addSpacerItem( new QSpacerItem(40, 1, QSizePolicy::Expanding, QSizePolicy::Fixed) );

   // setup the scene tree container widget
   m_fsTree = new QTreeWidget( dockWidgetContents );
   layout->addWidget( m_fsTree );

   QStringList columnLabels; 
   columnLabels.push_back( "Name" );
   columnLabels.push_back( "Size" );
   m_fsTree->setColumnCount( columnLabels.size() );
   m_fsTree->setHeaderLabels( columnLabels );
   m_fsTree->setDragEnabled( true ); 
   m_fsTree->setDropIndicatorShown( true ); 
   connect( m_fsTree, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( editResource( QTreeWidgetItem*, int ) ) );

   m_rootDir = new FSTreeEntry( m_fsTree, m_rm->getFilesystem() );
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
   FSTreeEntry* entry = find( rootDir );
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

ResourcesBrowser::FSTreeEntry* ResourcesBrowser::find( const std::string& dir )
{
   std::vector< std::string > pathParts;
   StringUtils::tokenize( dir, "/", pathParts );
   if ( pathParts.empty() )
   {
      return m_rootDir;
   }

   FSTreeEntry* currItem = m_rootDir;
   unsigned int count = pathParts.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      std::string currPathPart = pathParts[i] + "/";
      FSTreeEntry* nextItem = currItem->find( currPathPart );
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

void ResourcesBrowser::onDirectory( const std::string& name )
{
   ASSERT( m_rm != NULL, "This method can only be called when ResourcesManager instance is available" );

   const Filesystem& fs = m_rm->getFilesystem();

   std::string parentDirName = fs.extractDir( name );
   parentDirName = fs.toRelativePath( parentDirName );
   std::string newNodeName = fs.extractNodeName( name );

   FSTreeEntry* parent = find( parentDirName );
   ASSERT( parent != NULL, "Parent directory not found" );
   if ( parent )
   { 
      new FSTreeEntry( newNodeName, true, parent, fs );
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
   parentDirName = fs.toRelativePath( parentDirName );
   std::string newNodeName = fs.extractNodeName( name );

   FSTreeEntry* parent = find( parentDirName );
   ASSERT( parent != NULL, "Parent directory not found" );
   if ( parent )
   {  
      new FSTreeEntry( newNodeName, false, parent, fs );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::editResource( QTreeWidgetItem* item, int column )
{
   FSTreeEntry* entry = dynamic_cast< FSTreeEntry* >( item );
   if ( !entry || entry->isDir() )
   {
      return;
   }

   std::string fileName = entry->getRelativePath();

   ProgressDialog progressDlg;
   progressDlg.setProgress( 0 );
   Resource& resource = m_rm->create( fileName );
   progressDlg.setProgress( 1 );

   ResourceEditor* editor = create( resource );
   if ( editor )
   {
      editor->initialize( *m_mgr );
   }
   // CAUTION: we don't manage the editor's lifetime - it has to take care of itself
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::toggleFilesFiltering( bool )
{
   // change the view mode flag
   m_viewResourcesOnly = !m_viewResourcesOnly;

   // update the icon on the view type toggling button
   const Filesystem& fs = m_rm->getFilesystem();
   std::string iconsDir = fs.getShortcut( "editorIcons" );
   std::string iconName;
   if ( m_viewResourcesOnly )
   {
      iconName = iconsDir + "resource.png";
      m_toggleFileTypesViewBtn->setToolTip( "Switch to viewing all files" );
   }
   else
   {
      iconName = iconsDir + "fileInDir.png";
      m_toggleFileTypesViewBtn->setToolTip( "Switch to viewing resources only" );
   }
   m_toggleFileTypesViewBtn->setIcon( QIcon( iconName.c_str() ) );

   // refresh the view
   m_rootDir->clear();
   refresh();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ResourcesBrowser::FSTreeEntry::FSTreeEntry( QTreeWidget* hostTree, const Filesystem& fs )
: QTreeWidgetItem( hostTree )
, m_fsNodeName( "/" )
, m_isDir( true )
{
   setEntryIcon( fs );
   setEntryName( fs );
   setEntrySize( fs );
}

///////////////////////////////////////////////////////////////////////////////

ResourcesBrowser::FSTreeEntry::FSTreeEntry( const std::string& nodeName,
                                           bool isDir,
                                           QTreeWidgetItem* parent,
                                           const Filesystem& fs )
                                           : QTreeWidgetItem( parent ) 
                                           , m_fsNodeName( nodeName )
                                           , m_isDir( isDir )
{
   ASSERT( m_fsNodeName.length() > 0, "Invalid filesystem node name" );

   setEntryIcon( fs );
   setEntryName( fs );
   setEntrySize( fs );
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::FSTreeEntry::clear()
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      removeChild( child( 0 ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::FSTreeEntry::setEntryIcon( const Filesystem& fs )
{
   std::string iconsDir = fs.getShortcut( "editorIcons" );
   if ( m_isDir )
   {
      setIcon( 0, QIcon( ( iconsDir + "dirIcon.png" ).c_str() ) );
   }
   else
   {
      // check the extension look for a matching icon.
      // Icons for the specific extensions use the filename format:
      //       <<ext>>Icon.png

      std::string extension = fs.extractExtension( m_fsNodeName ).c_str();
      std::string absIconName = iconsDir + extension + "Icon.png";
      std::string relativeIconName = fs.toRelativePath( absIconName );

      if ( fs.doesExist( relativeIconName ) )
      {
         setIcon( 0, QIcon( absIconName.c_str() ) );
      }
      else
      {
         setIcon( 0, QIcon( ( iconsDir + "unknownFileIcon.png" ).c_str() ) );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::FSTreeEntry::setEntryName( const Filesystem& fs )
{
   if ( m_isDir )
   {
      unsigned int nameLen = m_fsNodeName.length();
      std::string nameWithoutSlash = m_fsNodeName.substr( 0, nameLen - 1 );
      setText( 0, nameWithoutSlash.c_str() );
   }
   else
   {
      setText( 0, m_fsNodeName.c_str() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesBrowser::FSTreeEntry::setEntrySize( const Filesystem& fs )
{
   if ( m_isDir )
   {
      setText( 1, "DIR" );
   }
   else
   {
      std::string relativePath = getRelativePath();

      std::size_t fileSize = 0;
      try
      {
         File* file = fs.open( relativePath );
         fileSize = file->size();
         delete file;
      }
      catch ( std::exception& ex)
      {
      }

      char fileSizeStr[ 32 ];
      sprintf_s( fileSizeStr, "%d", fileSize );
      setText( 1, fileSizeStr );
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string ResourcesBrowser::FSTreeEntry::getRelativePath() const
{
   // get the hierarchy of nodes leading up th this entry
   std::list< const FSTreeEntry* > entriesList;
   entriesList.push_front( this );

   while ( entriesList.front()->parent() != NULL )
   {
      entriesList.push_front( dynamic_cast< const FSTreeEntry* >( entriesList.front()->parent() ) );
   }

   // concatenate the full path from the hierarchy
   std::string path = "";
   for ( std::list< const FSTreeEntry* >::const_iterator it = entriesList.begin();
      it != entriesList.end(); ++it )
   {
      path += ( *it )->m_fsNodeName;
   }

   return path;
}

///////////////////////////////////////////////////////////////////////////////

ResourcesBrowser::FSTreeEntry* ResourcesBrowser::FSTreeEntry::find( const std::string& nodeName )
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      FSTreeEntry* entry = dynamic_cast< FSTreeEntry* >( child( i ) );
      if ( entry && entry->m_fsNodeName == nodeName )
      {
         return entry;
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
