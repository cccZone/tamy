#include "SceneEditor.h"
#include "core-MVC.h"
#include "tamyeditor.h"
#include "MainAppComponent.h"
#include "core-AppFlow.h"
#include "core-MVC.h"
#include <QProgressBar.h>
#include <QMessageBox.h>
#include <QSplitter.h>
#include <QSettings.h>
#include <QScrollArea.h>
#include "QPropertiesView.h"
#include "progressDialog.h"
#include "TamySceneWidget.h"


///////////////////////////////////////////////////////////////////////////////

SceneEditor::SceneEditor( Model& scene )
: m_scene( scene )
, m_sceneTrack( NULL )
, m_actionRun( NULL )
, m_scrollableDockWidgetContents( NULL )
, m_selectedEntityView( NULL )
, m_playing( false )
{
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::onInitialize()
{
   TamyEditor& mainEditor = TamyEditor::getInstance();

   TimeController& timeController = TamyEditor::getInstance().getTimeController();
   m_sceneTrack = &timeController.add( "SceneEditor" );

   // initialize user interface
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   QString iconsDir = resMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();
   
   // setup the main layout
   QVBoxLayout* mainLayout = new QVBoxLayout( this );
   setLayout( mainLayout );

   // add the toolbar
   {
      QToolBar* toolBar = new QToolBar( this );
      mainLayout->addWidget( toolBar );

      // resource management actions
      QAction* actionSaveScene = new QAction( QIcon( iconsDir + tr( "/saveFile.png" ) ), tr( "Save Scene" ), toolBar );
      toolBar->addAction( actionSaveScene );
      connect( actionSaveScene, SIGNAL( triggered() ), this, SLOT( saveScene() ) );

      toolBar->addSeparator();

      // scene execution commands
      m_runSceneIcon = QIcon( iconsDir + tr( "/play.png" ) );
      m_stopSceneIcon = QIcon( iconsDir + tr( "/stop.png" ) );

      m_actionRun = new QAction( m_runSceneIcon, tr( "Run" ), toolBar );
      m_actionRun->setShortcut( QKeySequence( tr( "F5" ) ) );
      toolBar->addAction( m_actionRun );
      connect( m_actionRun, SIGNAL( triggered() ), this, SLOT( toggleSceneExecution() ) );
   }

   // add the splitter that will host the render window and the properties browser
   {
      QSplitter* sceneViewSplitter = new QSplitter( Qt::Horizontal, this );
      mainLayout->addWidget( sceneViewSplitter );

      // add the scene preview
      QSettings& settings = mainEditor.getSettings();
      settings.beginGroup( "DefaultRenderer" );
      std::string defaultRendererPipelineName = settings.value( "pipeline", "" ).toString().toStdString();
      settings.endGroup();

      TamySceneWidget* sceneWidget = new TamySceneWidget( sceneViewSplitter, 0, defaultRendererPipelineName, mainEditor.getTimeController() );
      sceneWidget->setScene( m_scene );
      sceneViewSplitter->addWidget( sceneWidget );
      sceneViewSplitter->setStretchFactor( 0, 1 );

      // add the properties browser
      m_scrollableDockWidgetContents = new QScrollArea( sceneViewSplitter );
      m_scrollableDockWidgetContents->setObjectName( ( m_scene.getFilePath() + "/PropertiesEditor/ScrollableDockWidgetContents" ).c_str() );
      m_scrollableDockWidgetContents->setWidgetResizable( true );
      sceneViewSplitter->addWidget( m_scrollableDockWidgetContents );
      sceneViewSplitter->setStretchFactor( 1, 0 );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::closeEvent( QCloseEvent *event )
{
   // stop any scene that may be running
   stopScene();
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::toggleSceneExecution()
{
   if ( m_playing )
   {
      // the scene is running
      stopScene();
   }
   else
   {
      runScene();
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::stopScene()
{
   m_sceneTrack->remove( m_scene );

   // update the UI
   m_actionRun->setIcon( m_runSceneIcon );
   m_actionRun->setText( "Run" );

   m_playing = false;
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::runScene()
{
   m_sceneTrack->add( m_scene );

   // update the UI
   m_actionRun->setIcon( m_stopSceneIcon );
   m_actionRun->setText( "Stop" );

   m_playing = true;
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::saveScene()
{
   try
   {
      ProgressDialog progressObserver( this );
      progressObserver.initialize( "Mapping the dependencies", 1 );
      ExternalDependenciesSet externalDependencies;
      m_scene.saveResource( externalDependencies );

      progressObserver.advance();
      progressObserver.initialize( "Saving the dependencies", externalDependencies.size() );

      for ( unsigned int i = 0; i < externalDependencies.size(); ++i )
      {
         externalDependencies[ i ]->saveResource( externalDependencies );
         float progress = (float)i / (float)externalDependencies.size();
         progressObserver.advance();
      }

   }
   catch (std::exception& ex)
   {
      QMessageBox::warning( this, QString("Error occurred while saving scene"), ex.what(), QMessageBox::Ok );

      return;
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::onEntitySelected( Entity& entity )
{
   if ( m_selectedEntityView != NULL )
   {
      m_scrollableDockWidgetContents->setWidget( NULL );
      delete m_selectedEntityView;
   }

   m_selectedEntityView = new QPropertiesView();
   m_scrollableDockWidgetContents->setWidget( m_selectedEntityView );
   entity.viewProperties( *m_selectedEntityView );
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::onEntityDeselected()
{
   if ( m_selectedEntityView != NULL )
   {
      m_scrollableDockWidgetContents->setWidget( NULL );
      delete m_selectedEntityView;
      m_selectedEntityView = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
