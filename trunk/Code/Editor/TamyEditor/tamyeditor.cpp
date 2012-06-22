#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "core-Renderer.h"
#include "core-MVC.h"
#include "progressdialog.h"
#include <QTimer.h>
#include <QSettings.h>
#include <QCloseEvent>
#include <QDockWidget.h>
#include <QTreeWidget.h>
#include <QTabWidget>
#include <QSplitter>
#include <QFrame>
#include <QVBoxLayout>
#include "ResourcesBrowser.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include "core-AI.h"
#include "SplittableTabWidget.h"

// importers
#include "ml-IWF.h"
#include "ml-Blender.h"
#include "ml-BVH.h"

// editors
#include "SceneEditor.h"
#include "PixelShaderEditor.h"
#include "VertexShaderEditor.h"
#include "FragmentShaderEditor.h"
#include "RenderingPipelineEditor.h"
#include "SkeletonAnimationEditor.h"
#include "TextureEditor.h"
#include "MaterialEditor.h"
#include "GeometryShaderEditor.h"

// resources
#include "RenderingPipelineLayout.h"
#include "MaterialLayout.h"
#include "GeometryShaderLayout.h"


///////////////////////////////////////////////////////////////////////////////

TamyEditor* TamyEditor::s_theInstance = NULL;

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::createInstance( QApplication& app, const char* fsRoot, QWidget *parent, Qt::WFlags flags )
{
   delete s_theInstance;
   s_theInstance = new TamyEditor( app, fsRoot, parent, flags );
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::destroyInstance()
{
   delete s_theInstance;
   s_theInstance = NULL;

   // delete all resources
   ResourcesManager::getInstance().reset();
}

///////////////////////////////////////////////////////////////////////////////

TamyEditor::TamyEditor( QApplication& app, const char* fsRoot, QWidget *parent, Qt::WFlags flags )
: QMainWindow( parent, flags )
, m_mainTime( new CTimer() )
{
   ui.setupUi( this );

   setupResourcesManager( fsRoot );

   // add the resources browser
   {
      ResourcesBrowser* resourcesManagerFrame = new ResourcesBrowser( this );
      addDockWidget( Qt::LeftDockWidgetArea, resourcesManagerFrame );
   }

   // add the editors tabs
   {
      m_editorsTabs = new SplittableTabWidget( ui.renderWindow );
      connect( m_editorsTabs, SIGNAL( onTabClosed( QWidget* ) ), this, SLOT( onEditorTabClosed( QWidget* ) ) );
      ui.renderWindow->layout()->addWidget( m_editorsTabs );
   }

   // create the timer
   m_mainTimeSlot = new QTimer( this );
   connect( m_mainTimeSlot, SIGNAL( timeout() ), this, SLOT( updateMain() ) );
   m_mainTimeSlot->start( 1 );

   // load the settings file
   m_editorSettings = new QSettings( ( std::string( fsRoot ) + "/Editor/Settings.ini" ).c_str(), QSettings::IniFormat );

   // create the main subsystems
   m_timeController = new TimeController();

   // create the UI settings manager
   m_uiSettings = new QSettings( "Coversion", "TamyEditor" );
   serializeUISettings( false );

   // associate resources with their respective editors
   associate< Model, SceneEditor >();
   associate< PixelShader, PixelShaderEditor >();
   associate< VertexShader, VertexShaderEditor >();
   associate< FragmentShader, FragmentShaderEditor >();
   associate< RenderingPipelineLayout, RenderingPipelineEditor >();
   associate< MaterialLayout, MaterialEditor >();
   associate< GeometryShaderLayout, GeometryShaderEditor >();
   associate< SkeletonAnimation, SkeletonAnimationEditor >();
   associate< Texture, TextureEditor >();
}

///////////////////////////////////////////////////////////////////////////////

TamyEditor::~TamyEditor()
{
   // remove all editors
   std::vector< QWidget* > widgets;
   m_editorsTabs->collectPageWidgets( widgets );
   m_editorsTabs->clear();

   uint editorsCount = widgets.size();
   for ( unsigned int i = 0; i < editorsCount; ++i )
   {
      ResourceEditor* editor = dynamic_cast< ResourceEditor* >( widgets[i] );
      if ( editor )
      {
         editor->deinitialize( true );
      }
      delete widgets[i];
   }

   // delete the subsystems
   delete m_timeController; m_timeController = NULL;
   delete m_mainTime; m_mainTime = NULL;
   delete m_mainTimeSlot; m_mainTimeSlot = NULL;
   delete m_uiSettings; m_uiSettings = NULL;
   delete m_editorSettings; m_editorSettings = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::setupResourcesManager( const char* fsRoot )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   // setup the file system - it's paramount this works before anything in the application gets set up
   Filesystem* fs = new Filesystem( fsRoot );
   fs->setShortcut( "editorIcons", "/Editor/Icons/" );
   resMgr.setFilesystem( fs );
   fs->attach( *this );

   // register external resources
   resMgr.addImporter< BVHModelLoader, Model >( "bvh" );
   resMgr.addImporter< BVHSkeletonAnimationLoader, SkeletonAnimation >( "bvh" );
   resMgr.addImporter< IWFScene, Model >( "iwf" );
   resMgr.addImporter< BlenderScene, Model >( "dae" );
   resMgr.setProgressObserver< ProgressDialog >();
}

///////////////////////////////////////////////////////////////////////////////

QMenu& TamyEditor::getFileMenu()
{
   return *(ui.menuFile);
}

///////////////////////////////////////////////////////////////////////////////

QMenu& TamyEditor::getViewMenu()
{
   return *(ui.menuView);
}

///////////////////////////////////////////////////////////////////////////////

QToolBar& TamyEditor::getToolBar()
{
   return *(ui.mainToolBar);
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::updateMain()
{
   m_mainTime->tick();
   float timeElapsed = m_mainTime->getTimeElapsed();

   m_timeController->update( timeElapsed );
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::closeEvent( QCloseEvent *event )
{
   // serialize the settings
   serializeUISettings( true );

   // accept the event
   event->accept();
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::serializeUISettings( bool save )
{
   std::list< QWidget* >   widgetsQueue;
   widgetsQueue.push_back( this );

   while( !widgetsQueue.empty() )
   {
      QWidget* currWidget = widgetsQueue.front();
      widgetsQueue.pop_front();

      serializeWidgetSettings( *currWidget, save );

      // analyze the widget's children
      const QObjectList& children = currWidget->children();
      foreach( QObject* obj, children )
      {
         QWidget* childWidget = dynamic_cast< QWidget* >( obj );
         if ( childWidget )
         {
            widgetsQueue.push_back( childWidget );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::serializeWidgetSettings( QWidget& widget, bool save )
{
   QString objName = widget.objectName();
   if ( objName.isEmpty() )
   {
      return;
   }
   m_uiSettings->beginGroup( objName );

   // serialize common widget settings
   if ( save )
   {
      m_uiSettings->setValue( "size", widget.size() );
      m_uiSettings->setValue( "pos", widget.pos() );
   }
   else
   {
      widget.resize( m_uiSettings->value( "size", widget.size() ).toSize() );
      widget.move( m_uiSettings->value( "pos", widget.pos() ).toPoint() );
   }

   // serialize type specific settings (non-exclusive ifs block)
   if ( dynamic_cast< QDockWidget* >( &widget ) )
   {
      serializeDockWidgetSettings( *dynamic_cast< QDockWidget* >( &widget ), save );
   }

   if ( dynamic_cast< QTreeWidget* >( &widget ) )
   {
      serializeTreeWidgetSettings( *dynamic_cast< QTreeWidget* >( &widget ), save );
   }

   // close the widget's settings group
   m_uiSettings->endGroup();
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::serializeDockWidgetSettings( QDockWidget& widget, bool save )
{
   if ( save )
   {
      m_uiSettings->setValue( "features", (int)widget.features() );
      m_uiSettings->setValue( "floating", widget.isFloating() );
   }
   else
   {
      widget.setFeatures( ( QDockWidget::DockWidgetFeatures )m_uiSettings->value( "features", (int)widget.features() ).toInt() );
      widget.setFloating( m_uiSettings->value( "floating", widget.isFloating() ).toBool() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::serializeTreeWidgetSettings( QTreeWidget& widget, bool save )
{
   // gather info about present columns count
   QList< QVariant > columnSizes;
   int count = widget.columnCount();
   for ( int i = 0; i < count; ++i )
   {
      columnSizes.push_back( widget.columnWidth( i ) );
   }

   if ( save )
   {
      m_uiSettings->setValue( "columnSizes", columnSizes );
   }
   else
   {
      columnSizes = m_uiSettings->value( "columnSizes", columnSizes ).toList();

      // set the column sizes
      widget.setColumnCount( columnSizes.size() );
      int colIdx = 0;
      foreach( QVariant width, columnSizes )
      {
         widget.setColumnWidth( colIdx++, width.toInt() );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::editResource( Resource& resource, const QIcon& icon )
{
   // first - look for a tab with the same path - if there is one, it means that we're
   // already editing this resource
   SplittableTabLocation location;
   bool found = m_editorsTabs->findTabByName( resource.getFilePath().c_str(), location );
   if ( found )
   {
      // yep - we're already editing it. Focus on it and that's it
      m_editorsTabs->setActiveTab( location );
      return;
   }

   // if we got this far, it means that we need a new editor to edit this resource
   ResourceEditor* editor = GenericFactory< Resource, ResourceEditor >::create( resource );
   if ( editor )
   {
      editor->initialize( resource.getFilePath().c_str(), icon );
      m_editorsTabs->addTab( editor, editor->getIcon(), editor->getLabel() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::onEditorTabClosed( QWidget* editorWidget )
{
   ResourceEditor* editor = dynamic_cast< ResourceEditor* >( editorWidget );
   if ( editor )
   {
      editor->deinitialize( false );
      delete editor;
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::onDirChanged( const FilePath& dir )
{
   const Filesystem& fs = ResourcesManager::getInstance().getFilesystem();

   // if any of the resource editors are editing a resource from the deleted directory, close them
   // immediately
   std::vector< QWidget* > widgets;
   std::vector< SplittableTabLocation > tabsLocations;
   m_editorsTabs->collectPageWidgets( widgets, &tabsLocations );

   uint editorsCount = widgets.size();
   for ( uint i = 0; i < editorsCount; ++i )
   {
      QWidget* editorWidget = widgets[i];
      ResourceEditor* editor = dynamic_cast< ResourceEditor* >( editorWidget );
      if ( !editor )
      {
         continue;
      }

      FilePath resourcePath( editor->getLabel().toStdString() );

      if ( editor && resourcePath.isSubPath( dir ) && !fs.doesExist( resourcePath ) )
      {
         editor->deinitialize( false );
         m_editorsTabs->removeTab( tabsLocations[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::onFileEdited( const FilePath& path )
{
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::onFileRemoved( const FilePath& path )
{
   const Filesystem& fs = ResourcesManager::getInstance().getFilesystem();

   // if any of the resource editors are editing a deleted resource, close them
   // immediately
   std::vector< QWidget* > widgets;
   std::vector< SplittableTabLocation > tabsLocations;
   m_editorsTabs->collectPageWidgets( widgets, &tabsLocations );

   uint editorsCount = widgets.size();
   for ( uint i = 0; i < editorsCount; ++i )
   {
      QWidget* editorWidget = widgets[i];
      ResourceEditor* editor = dynamic_cast< ResourceEditor* >( editorWidget );
      if ( !editor )
      {
         continue;
      }

      QString resourcePath = editor->getLabel();

      if ( editor && path == FilePath( resourcePath.toStdString() ) )
      {
         editor->deinitialize( false );
         m_editorsTabs->removeTab( tabsLocations[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
