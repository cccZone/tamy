#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "core-Renderer.h"
#include "core-MVC.h"
#include "core-AI.h"
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
#include <QPalette>
#include "ResourcesBrowser.h"
#include "MainEditorPanel.h"
#include "UISerializationUtil.h"

// importers
#include "ml-IWF.h"
#include "ml-Collada.h"
#include "ml-BVH.h"
#include "ml-FBX.h"

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
#include "ProjectEditor.h"

// resources
#include "RenderingPipelineLayout.h"
#include "MaterialLayout.h"
#include "GeometryShaderLayout.h"
#include "ResourceManagementUtil.h"
#include "Project.h"

// tools
#include "ProfilerFrame.h"
#include "core/Profiler.h"


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
   , m_app( app )
   , m_mainTime( new CTimer() )
   , m_resourcesBrowser( NULL )
   , m_activeProject( NULL )
   , m_activeProfilerView( NULL )
{
   ui.setupUi( this );
   setAutoFillBackground( true );

   setupResourcesManager( fsRoot );

   // add the resources browser
   {
      m_resourcesBrowser = new ResourcesBrowser( this );
      addDockWidget( Qt::LeftDockWidgetArea, m_resourcesBrowser );
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
   associate< Project, ProjectEditor >();

   // setup menu contents
   {
      QAction* startProfilerAction = new QAction( "Profiler", ui.menuView );
      connect( startProfilerAction, SIGNAL( triggered() ), this, SLOT( startProfiler() ) );
      ui.menuView->addAction( startProfilerAction );
   }
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
   resMgr.addImporter< FBXSceneLoader, Model >( "fbx" );
   resMgr.addImporter< BVHModelLoader, Model >( "bvh" );
   resMgr.addImporter< BVHSkeletonAnimationLoader, SkeletonAnimation >( "bvh" );
   resMgr.addImporter< IWFScene, Model >( "iwf" );
   resMgr.addImporter< ColladaScene, Model >( "dae" );
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
   // tick the main timer
   m_mainTime->tick();
   float timeElapsed = m_mainTime->getTimeElapsed();

   // gather the profiling information
   if ( m_activeProfilerView )
   {
      Profiler::getInstance().endFrame();
      m_activeProfilerView->update( timeElapsed );
      Profiler::getInstance().beginFrame();
   }


   // update engine's main loop
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
   // no editor can be created without an active project
   if ( m_activeProject == NULL )
   {
      return NULL;
   }

   // and even if there's an active project, the edited resource must be a part of it
   FilePath resourcePath = resource->getFilePath();
   if ( m_activeProject->isMember( resourcePath ) == false )
   {
      return NULL;
   }

   // if we got this far, it means that we need a new editor to edit this resource
   ResourceEditor* editor = GenericFactory< Resource, ResourceEditor >::create( *resource );
   if ( editor )
   {
      editor->initialize( resourcePath.c_str(), icon );
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

   // this may have been the profiler
   if ( editorWidget == m_activeProfilerView )
   {
      m_activeProfilerView = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::onDirAdded( const FilePath& dir )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::onDirRemoved( const FilePath& dir )
{
   if ( m_activeProject )
   {
      // if the removed directory was where the active project was located, we need to close the project
      if ( m_activeProjectPath.isSubPath( dir ) )
      {
         // yes it was - close the project
         setActiveProject( NULL );
      }
   }
   else
   {
      closeEditors( dir, PathTest::FROM_METHOD( TamyEditor, isResourceFromDeletedDir, this ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::onFileEdited( const FilePath& path )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::onFileRemoved( const FilePath& path )
{
   // if the removed file was the active project resource, close the project
   if ( m_activeProject && m_activeProjectPath == path )
   {
      // yes it was - close the project
      setActiveProject( NULL );
   }
   else
   {
      // and close any editors tied to this particular resource ( Project may also have an editor assigned to it and it may be open at the moment )
      closeEditors( path, PathTest::FROM_METHOD( TamyEditor, isSamePath, this ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::closeEditors( const FilePath& dir, const PathTest& test )
{
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

      if ( editor && test( resourcePath, dir ) )
      {
         editor->deinitialize( false );
         m_editorsTabs->removeTab( tabsLocations[i] );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

bool TamyEditor::isSamePath( const FilePath& resourcePath, const FilePath& checkedPath )
{
   return resourcePath == checkedPath;
}

///////////////////////////////////////////////////////////////////////////////

bool TamyEditor::isParentDir( const FilePath& resourcePath, const FilePath& dir )
{
   return resourcePath.isSubPath( dir );
}

///////////////////////////////////////////////////////////////////////////////

bool TamyEditor::isResourceFromDeletedDir( const FilePath& resourcePath, const FilePath& deletedDir )
{
   const Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   return resourcePath.isSubPath( deletedDir ) && !fs.doesExist( resourcePath );
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::setActiveProject( Project* project )
{
   if ( m_activeProject == project )
   {
      // nothing's changed
      return;
   }

   // close all active editors
   closeEditors( FilePath( "/" ), PathTest::FROM_METHOD( TamyEditor, allTrue, this ) );

   // set the new project as the active one
   m_activeProject = project;

   // memorize project's path
   // set the project path
   if ( m_activeProject )
   {
      m_activeProjectPath = m_activeProject->getFilePath();
   }
   else
   {
      m_activeProjectPath = FilePath();
   }

   // notify the resources browser about it
   m_resourcesBrowser->setActiveProject( m_activeProject );
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::startProfiler()
{
   if ( m_activeProfilerView )
   {
      // a profiler is already active
      return;
   }

   m_activeProfilerView = new ProfilerFrame();
   m_editorsTabs->addEditor( m_activeProfilerView, QIcon(), QString( "Profiler" ) );
}

///////////////////////////////////////////////////////////////////////////////
