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
#include "MainEditorPanel.h"
#include "UISerializationUtil.h"

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

   // serialize UI layout
   UISerializationUtil::serialize( s_theInstance, s_theInstance->m_editorsTabs, false );
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::destroyInstance()
{
   // deserialize UI layout
   UISerializationUtil::serialize( s_theInstance, s_theInstance->m_editorsTabs, true );

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
      m_editorsTabs = new MainEditorPanel( ui.renderWindow, this );
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
   std::vector< ResourceEditor* > editors;
   m_editorsTabs->collectEditors( editors );
   m_editorsTabs->clear();

   uint editorsCount = editors.size();
   for ( unsigned int i = 0; i < editorsCount; ++i )
   {
      ResourceEditor* editor = editors[i];
      if ( editor )
      {
         editor->deinitialize( true );
      }
      delete editor;
   }

   // delete the subsystems
   delete m_timeController; m_timeController = NULL;
   delete m_mainTime; m_mainTime = NULL;
   delete m_mainTimeSlot; m_mainTimeSlot = NULL;
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
   // accept the event
   event->accept();
}

///////////////////////////////////////////////////////////////////////////////

bool TamyEditor::activateResourceEditor( Resource* resource )
{
   // first - look for a tab with the same path - if there is one, it means that we're
   // already editing this resource
   TabLocation location;
   bool found = m_editorsTabs->findTabByName( resource->getFilePath().c_str(), location );
   if ( found )
   {
      // yep - we're already editing it. Focus on it and that's it
      m_editorsTabs->setActiveTab( location );
   }

   return found;
}

///////////////////////////////////////////////////////////////////////////////

ResourceEditor* TamyEditor::createResourceEditor( Resource* resource, const QIcon& icon )
{
   // if we got this far, it means that we need a new editor to edit this resource
   ResourceEditor* editor = GenericFactory< Resource, ResourceEditor >::create( *resource );
   if ( editor )
   {
      editor->initialize( resource->getFilePath().c_str(), icon );
   }
   return editor;
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::addResourceEditor( ResourceEditor* editor, Qt::DockWidgetArea dockArea )
{
   if ( editor != NULL )
   {
      m_editorsTabs->addEditor( editor, editor->getIcon(), editor->getLabel(), dockArea );
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
   std::vector< ResourceEditor* > editors;
   std::vector< TabLocation > tabsLocations;
   m_editorsTabs->collectEditors( editors, &tabsLocations );

   uint editorsCount = editors.size();
   for ( uint i = 0; i < editorsCount; ++i )
   {
      ResourceEditor* editor = editors[i];
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
   std::vector< ResourceEditor* > editors;
   std::vector< TabLocation > tabsLocations;
   m_editorsTabs->collectEditors( editors, &tabsLocations );

   uint editorsCount = editors.size();
   for ( uint i = 0; i < editorsCount; ++i )
   {
      ResourceEditor* editor = editors[i];
      QString resourcePath = editor->getLabel();

      if ( editor && path == FilePath( resourcePath.toStdString() ) )
      {
         editor->deinitialize( false );
         m_editorsTabs->removeTab( tabsLocations[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
