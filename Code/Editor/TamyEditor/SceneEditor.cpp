#include "SceneEditor.h"
#include "core-MVC.h"
#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "core-MVC.h"
#include <QProgressBar.h>
#include <QMessageBox.h>
#include <QSplitter.h>
#include <QSettings.h>
#include <QScrollArea.h>
#include "SelectedEntityPropertiesViewer.h"
#include "progressDialog.h"
#include "TamySceneWidget.h"
#include "CameraMovementController.h"
#include "SceneTreeViewer.h"
#include "SelectionManager.h"
#include "SceneObjectsManipulator.h"
#include "NodeTransformController.h"


///////////////////////////////////////////////////////////////////////////////

SceneEditor::SceneEditor( Model& scene )
   : m_scene( scene )
   , m_sceneTrack( NULL )
   , m_actionRun( NULL )
   , m_sceneWidget( NULL )
   , m_sceneTreeViewer( NULL )
   , m_selectionManager( new SelectionManager() )
   , m_playing( false )
   , m_sceneObjectsManipulator( NULL )
   , m_objectsManipulationMode( NTM_TRANSLATE )
{
   m_scene.attach( *m_selectionManager );
}

///////////////////////////////////////////////////////////////////////////////

SceneEditor::~SceneEditor()
{
   // gonna get disposed of by the rendering widget
   m_sceneObjectsManipulator = NULL;

   if ( m_sceneTreeViewer )
   {
      m_scene.detach( *m_sceneTreeViewer );
   }

   // dispose of the selection manager
   m_scene.detach( *m_selectionManager );
   delete m_selectionManager;
   m_selectionManager = NULL;
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
      {
         QAction* actionSaveScene = new QAction( QIcon( iconsDir + tr( "/saveFile.png" ) ), tr( "Save Scene" ), toolBar );
         toolBar->addAction( actionSaveScene );
         connect( actionSaveScene, SIGNAL( triggered() ), this, SLOT( saveScene() ) );

         toolBar->addSeparator();
      }

      // scene execution commands
      {
         m_runSceneIcon = QIcon( iconsDir + tr( "/play.png" ) );
         m_stopSceneIcon = QIcon( iconsDir + tr( "/stop.png" ) );

         m_actionRun = new QAction( m_runSceneIcon, tr( "Run" ), toolBar );
         m_actionRun->setShortcut( QKeySequence( tr( "F5" ) ) );
         toolBar->addAction( m_actionRun );
         connect( m_actionRun, SIGNAL( triggered() ), this, SLOT( toggleSceneExecution() ) );

         toolBar->addSeparator();
      }

      // object manipulation mode change commands
      {
         QActionGroup* actionGroup = new QActionGroup( toolBar );

         QAction* actionTranslate = new QAction( QIcon( iconsDir + tr( "/translate.png" ) ), tr( "Translate" ), toolBar );
         actionTranslate->setCheckable( true );
         toolBar->addAction( actionTranslate );
         actionGroup->addAction( actionTranslate );
         connect( actionTranslate, SIGNAL( triggered() ), this, SLOT( setNodeTranslateMode() ) );

         QAction* actionRotate = new QAction( QIcon( iconsDir + tr( "/rotate.png" ) ), tr( "Rotate" ), toolBar );
         actionRotate->setCheckable( true );
         toolBar->addAction( actionRotate );
         actionGroup->addAction( actionRotate );
         connect( actionRotate, SIGNAL( triggered() ), this, SLOT( setNodeRotateMode() ) );
         actionTranslate->setChecked( true );

         toolBar->addSeparator();
      }

      // debug stuff
      {

         QAction* actionDebug = new QAction( QIcon( iconsDir + tr( "/cog.png" ) ), tr( "Debug" ), toolBar );
         toolBar->addAction( actionDebug );
         connect( actionDebug, SIGNAL( triggered() ), this, SLOT( toggleDebugMode() ) );

         toolBar->addSeparator();
      }

   }

   // add the splitter that will host the render window and the browsers frames
   {
      QSplitter* sceneViewSplitter = new QSplitter( Qt::Horizontal, this );
      mainLayout->addWidget( sceneViewSplitter );

      // extract the camera from the scene widget and set up the scene tree viewer
      // so that each time we select a node, the camera focuses on it
      Camera* camera = NULL;

      // add the scene preview
      {
         QSettings& settings = mainEditor.getSettings();
         settings.beginGroup( "DefaultRenderer" );
         std::string defaultRendererPipelineName = settings.value( "pipeline", "" ).toString().toStdString();
         settings.endGroup();

         m_sceneWidget = new TamySceneWidget( sceneViewSplitter, 0, defaultRendererPipelineName, mainEditor.getTimeController() );
         camera = &m_sceneWidget->getCamera();
         m_sceneWidget->setScene( m_scene );
         sceneViewSplitter->addWidget( m_sceneWidget );
         sceneViewSplitter->setStretchFactor( 0, 1 );
         m_selectionManager->attach( *m_sceneWidget );
         
         // setup the manipulator
         m_sceneObjectsManipulator = new SceneObjectsManipulator( *this );
         m_sceneWidget->setInputController( m_sceneObjectsManipulator );
         m_selectionManager->attach( *m_sceneObjectsManipulator );
      }

      // add the browsers
      {
         QSplitter* browsersSplitter = new QSplitter( Qt::Vertical, sceneViewSplitter );
         sceneViewSplitter->addWidget( browsersSplitter );
         sceneViewSplitter->setStretchFactor( 1, 0 );

         {
            // add the properties browser
            SelectedEntityPropertiesViewer* propertiesEditor = new SelectedEntityPropertiesViewer( browsersSplitter );
            m_selectionManager->attach( *propertiesEditor );
            propertiesEditor->setObjectName( ( m_scene.getFilePath() + "/PropertiesEditor" ).c_str() );
            propertiesEditor->setWidgetResizable( true );
            browsersSplitter->addWidget( propertiesEditor );

            // add the scene tree browser
            m_sceneTreeViewer = new SceneTreeViewer( browsersSplitter );
            m_scene.attach( *m_sceneTreeViewer );
            m_selectionManager->attach( *m_sceneTreeViewer );
            m_sceneTreeViewer->setObjectName( ( m_scene.getFilePath() + "/SceneTreeViewer" ).c_str() );
            browsersSplitter->addWidget( m_sceneTreeViewer );
            m_sceneTreeViewer->setCamera( *camera );
            connect( m_sceneTreeViewer, SIGNAL( onSceneTreeObjectSelected( Entity& ) ), this, SLOT( entitySelected( Entity& ) ) );
            connect( m_sceneTreeViewer, SIGNAL( onSceneTreeSelectionCleaned() ), this, SLOT( selectionCleaned() ) );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::onDeinitialize()
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

void SceneEditor::entitySelected( Entity& entity )
{
   m_selectionManager->selectEntity( entity );
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::selectionCleaned()
{
   m_selectionManager->resetSelection();
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::setNodeTranslateMode()
{
   m_objectsManipulationMode = NTM_TRANSLATE;
   m_sceneObjectsManipulator->onSettingsChanged();
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::setNodeRotateMode()
{
   m_objectsManipulationMode = NTM_ROTATE;
   m_sceneObjectsManipulator->onSettingsChanged();
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::toggleDebugMode()
{
   m_sceneWidget->toggleDebugMode();
}

///////////////////////////////////////////////////////////////////////////////
