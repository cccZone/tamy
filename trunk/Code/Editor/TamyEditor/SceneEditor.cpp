#include "SceneEditor.h"
#include "core.h"
#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include <QProgressBar.h>
#include <QMessageBox.h>
#include <QSplitter.h>
#include <QSettings.h>
#include <QScrollArea.h>
#include <QPalette>
#include <QColor>

// scene rendering widget
#include "TamySceneWidget.h"
#include "CameraMovementController.h"
#include "SceneTreeViewer.h"
#include "SelectionManager.h"
#include "SceneObjectsManipulator.h"

// properties browser
#include "SelectedEntityPropertiesViewer.h"

// game deployment
#include "GameDeploymentFrame.h"
#include "GameRunner.h"

// drag & drop
#include "DropFrame.h"
#include "FSNodeMimeData.h"

// tools
#include "ClosableFrame.h"
#include "progressDialog.h"


///////////////////////////////////////////////////////////////////////////////

SceneEditor::SceneEditor( Model& scene )
   : m_scene( scene )
   , m_sceneTrack( NULL )
   , m_actionRun( NULL )
   , m_sceneWidget( NULL )
   , m_sceneTreeViewer( NULL )
   , m_selectionManager( NULL )
   , m_playing( false )
   , m_sceneObjectsManipulator( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

SceneEditor::~SceneEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::onInitialize()
{
   TamyEditor& mainEditor = TamyEditor::getInstance();
   
   TimeController& timeController = TamyEditor::getInstance().getTimeController();
   m_sceneTrack = &timeController.add( "SceneEditor" );

   // create a selection manager
   m_selectionManager = new SelectionManager();
   m_scene.attach( *m_selectionManager );

   // initialize user interface
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   QString iconsDir = resMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();
   
   // setup the main layout
   m_mainLayout = new QVBoxLayout( this );
   m_mainLayout->setContentsMargins(0, 0, 0, 0);
   setLayout( m_mainLayout );

   // add the toolbar
   {
      QToolBar* toolBar = new QToolBar( this );
      m_mainLayout->addWidget( toolBar );

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

      // deploy application
      {
         QAction* actionDeployGame = new QAction( QIcon( iconsDir + tr( "/deployGame.png" ) ), tr( "Deploy game" ), toolBar );
         toolBar->addAction( actionDeployGame );
         connect( actionDeployGame, SIGNAL( triggered() ), this, SLOT( showGameDeploymentFrame() ) );
      }

   }

   // add the splitter that will host the render window and the browsers frames
   {
      QSplitter* sceneViewSplitter = new QSplitter( Qt::Horizontal, this );
      m_mainLayout->addWidget( sceneViewSplitter, 1 );

      // extract the camera from the scene widget and set up the scene tree viewer
      // so that each time we select a node, the camera focuses on it
      Camera* camera = NULL;

      // add the scene preview
      {
         QSettings& settings = mainEditor.getSettings();
         settings.beginGroup( "DefaultRenderer" );
         std::string defaultRendererPipelineName = settings.value( "pipeline", "" ).toString().toStdString();
         settings.endGroup();

         DropFrame* sceneDropsFrame = new DropFrame( sceneViewSplitter, new FSNodeMimeData( m_droppedPaths ) );
         connect( sceneDropsFrame, SIGNAL( changed() ), this, SLOT( pathsDropped() ) );
         sceneViewSplitter->addWidget( sceneDropsFrame );
         sceneViewSplitter->setStretchFactor( 0, 1 );

         QHBoxLayout* sceneDropsFrameLayout = new QHBoxLayout();
         sceneDropsFrame->setLayout( sceneDropsFrameLayout );

         m_sceneWidget = new TamySceneWidget( sceneDropsFrame, 0, defaultRendererPipelineName, mainEditor.getTimeController() );
         camera = &m_sceneWidget->getCamera();
         m_sceneWidget->setScene( m_scene );
         sceneDropsFrameLayout->addWidget( m_sceneWidget );
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
            propertiesEditor->setObjectName( ( m_scene.getFilePath().getRelativePath() + "/PropertiesEditor" ).c_str() );
            propertiesEditor->setWidgetResizable( true );
            browsersSplitter->addWidget( propertiesEditor );

            // add the scene tree browser
            m_sceneTreeViewer = new SceneTreeViewer( browsersSplitter );
            m_scene.attach( *m_sceneTreeViewer );
            m_selectionManager->attach( *m_sceneTreeViewer );
            m_sceneTreeViewer->setObjectName( ( m_scene.getFilePath().getRelativePath() + "/SceneTreeViewer" ).c_str() );
            browsersSplitter->addWidget( m_sceneTreeViewer );
            m_sceneTreeViewer->setCamera( *camera );
            connect( m_sceneTreeViewer, SIGNAL( onSceneTreeObjectSelected( Entity& ) ), this, SLOT( entitySelected( Entity& ) ) );
            connect( m_sceneTreeViewer, SIGNAL( onSceneTreeSelectionCleaned() ), this, SLOT( selectionCleaned() ) );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::onDeinitialize( bool saveProgress )
{
   // stop any scene that may be running
   stopScene();

   // detach all active scene listeners
   if ( m_sceneTreeViewer )
   {
      m_scene.detach( *m_sceneTreeViewer );
   }
   if ( m_selectionManager )
   {
      m_scene.detach( *m_selectionManager );
      delete m_selectionManager;
      m_selectionManager = NULL;
   }

   // gonna get disposed of by the rendering widget
   m_sceneObjectsManipulator = NULL;
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

void SceneEditor::pathsDropped()
{
   ResourcesManager& rm = ResourcesManager::getInstance();

   unsigned int count = m_droppedPaths.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      FilePath path = m_droppedPaths[i];

      if ( path.extractExtension() == RenderingPipeline::getExtension() )
      {
         // if this is a rendering pipeline, switch it in the rendering widget
         m_sceneWidget->setRenderingPipeline( path );
      }
      else
      {
         TResourceImporter< Model >* importer = rm.createImporter< Model >( path );
         if ( importer )
         {
            importer->import( m_scene );
            delete importer;
         }
      }
   }

   m_droppedPaths.clear();
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::saveScene()
{
   try
   {
      ProgressDialog progressObserver( this );
      progressObserver.initialize( "Saving...", 1 );
      m_scene.saveResource();
      progressObserver.advance();
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
   m_sceneWidget->setGizmoTranslationMode();
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::setNodeRotateMode()
{
   m_sceneWidget->setGizmoRotationMode();
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::toggleDebugMode()
{
   m_sceneWidget->toggleDebugMode();
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::showGameDeploymentFrame()
{
   ClosableFrame* closableFrame = new ClosableFrame( this );
   connect( closableFrame, SIGNAL( onCloseRequest( QWidget* ) ), this, SLOT( onEmbedededWidgetClosed( QWidget* ) ) );

   GameDeploymentFrame* frame = new GameDeploymentFrame( closableFrame );
   connect( frame, SIGNAL( deployGame( QWidget*, const GameDeploymentInfo& ) ), this, SLOT( onDeployGame( QWidget*, const GameDeploymentInfo& ) ) );

   closableFrame->setWidget( frame );

   // simply embed the new frame in the main one at the top
   m_mainLayout->insertWidget( 1, closableFrame, 0 );
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::onEmbedededWidgetClosed( QWidget* closedWidget )
{
   m_mainLayout->removeWidget( closedWidget );

   // dispose of the widget
   closedWidget->setParent( NULL );
   closedWidget->deleteLater();
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::onDeployGame( QWidget* colorCodeWidget, const GameDeploymentInfo& info )
{
   // deploy the game
   GameRunner* gameRunner = NULL;
   {
      // create a progress dialog that will track the deployment process
      ProgressDialog* progressDialog = new ProgressDialog( this );

      gameRunner = deployGame( info, progressDialog );

      // cleanup
      delete progressDialog;
   }

   // color code the specified widget to indicate if the deployment went well or not
   {
      colorCodeWidget->setAutoFillBackground( true );
      QPalette palette = colorCodeWidget->palette();

      const QColor opSuccessfulColor( 135,255, 56 );
      const QColor opFailedColor( 255, 92, 43 );
      palette.setColor( QPalette::Button, ( gameRunner != NULL ) ? opSuccessfulColor : opFailedColor );

      colorCodeWidget->setPalette( palette );
   }

   // if the deployment was successful, and the user chose to - run the game
   if ( gameRunner && info.m_runAfterDeployment )
   {
      // create a game runner
      gameRunner->run();
   }

   // cleanup
   delete gameRunner;
}

///////////////////////////////////////////////////////////////////////////////

static bool CopyFileEnsurePath( const std::string& sourcePath, const std::string& targetPath )
{
   // create the target directory
   {
      std::vector< std::string > pathElements;
      StringUtils::tokenize( targetPath, "/\\", pathElements );

      std::string currentPath = pathElements[0];
      uint count = pathElements.size() - 1;
      for( uint i = 1; i < count; ++i )
      {
         currentPath += "\\" + pathElements[i];
         bool result = CreateDirectoryA( currentPath.c_str(), NULL );
         if ( !result )
         {
            DWORD errCode = GetLastError();
            if ( errCode != ERROR_ALREADY_EXISTS )
            {
               // the directory couldn't be created for some reason
               return false;
            }
         }
      }
   }

   // copy the file
   bool result = CopyFileA( sourcePath.c_str(), targetPath.c_str(), false );
   return result;
}

///////////////////////////////////////////////////////////////////////////////

GameRunner* SceneEditor::deployGame( const GameDeploymentInfo& info, ProgressDialog* progressDialog )
{
   // WHAT HAPPENS WHEN MORE PLATFORMS ARE IMPLEMENTED?
   // A lot's gonna change around here when Android, iPhone and other platforms appear, but for now let's keep this block in this method.
   // What's not gonna change is the fact the method's gonna return a GameRunner instance.


   if ( info.m_targetDir.empty() )
   {
      // cant' deploy to an unspecified directory
      return NULL;
   }

   ResourcesManager& resMgr = ResourcesManager::getInstance();
   Filesystem& fs = resMgr.getFilesystem();
   std::string DEPLOYMENT_RESOURCES_ROOT = info.m_targetDir + "/Data/";


   // get the rendering pipeline, 'cause it's one of the elements we're gonna be saving
   const FilePath& renderingPipelinePath = m_sceneWidget->getRenderingPipeline();
   RenderingPipeline* pipeline = resMgr.findResource< RenderingPipeline >( renderingPipelinePath );
   if ( !pipeline )
   {
      // the pipeline wasn't loaded - that's weird ( since we have the scene widget initialized and all ).
      // Investigate, but don't deploy anything.
      ASSERT_MSG( pipeline != NULL, "Rendering pipeline not defined" );

      return NULL;
   }

   // save the resources ( we want to save the state most up to date )
   {
      m_scene.saveResource();
      pipeline->saveResource();
   }

   // create the resources map that needs to be copied
   std::vector< FilePath > externalDependencies;
   {
      ReflectionSerializationUtil::collectExternalDependencies( &m_scene, externalDependencies );
      ReflectionSerializationUtil::collectExternalDependencies( pipeline, externalDependencies );
   }

   progressDialog->initialize( "Game deployment", externalDependencies.size() );

   // copy the resources to the "Data" directory
   {
      uint count = externalDependencies.size();
      bool deploymentSuccessful = true;
      for ( uint i = 0; i < count; ++i )
      {
         std::string sourcePath = externalDependencies[i].toAbsolutePath( fs );
         std::string targetPath = DEPLOYMENT_RESOURCES_ROOT + externalDependencies[i].c_str();
         deploymentSuccessful = CopyFileEnsurePath( sourcePath.c_str(), targetPath.c_str() );

         // advance the progress dialog
         progressDialog->advance();
      }

      if ( !deploymentSuccessful )
      {
         // something broke down - bail
         return NULL;
      }
   }

      
   // copy the selected game binary 
   std::string gameExePath = info.m_targetDir + "/game.exe";
   {
      std::string binSourcePath;
      switch( info.m_platform )
      {
      case GDP_WINDOWS_OPENGL:
         {
            binSourcePath = fs.getCurrRoot() + "/Deployment/GameWinOpenGL/game.exe";
            break;
         }

      case GDP_WINDOWS_DX9: // fallthrough
      default:
         {
            binSourcePath = fs.getCurrRoot() + "/Deployment/GameWinDX9/game.exe";
            break;
         }
      }

      bool deploymentSuccessful = CopyFileEnsurePath( binSourcePath.c_str(), gameExePath.c_str() );
      // advance the progress dialog
      progressDialog->advance();

      if ( !deploymentSuccessful )
      {
         // something broke down - bail
         return NULL;
      }
   }

   // save the configuration resource
   {
      GameConfig* config = new GameConfig();
      config->m_renderingPipelinePath = renderingPipelinePath;
      config->m_worldModelPath = m_scene.getFilePath();

      // save it in this filesystem
      FilePath gameConfigPath( "game.gcf" );
      ReflectionSerializationUtil::saveObject( config, gameConfigPath );

      // destroy the config instance
      delete config;

      // move it to the destination
      std::string gameConfigSourcePath = gameConfigPath.toAbsolutePath( fs );
      std::string gameConfigDestPath = DEPLOYMENT_RESOURCES_ROOT + "/" + gameConfigPath.c_str();
      bool deploymentSuccessful = CopyFileEnsurePath( gameConfigSourcePath, gameConfigDestPath ); 

      // delete the temporary file
      fs.remove( gameConfigPath );

      if ( !deploymentSuccessful )
      {
         // something broke down - bail
         return NULL;
      }

   }

   // create the runner
   GameRunner* runner = NULL;
   switch( info.m_platform )
   {
   case GDP_WINDOWS_OPENGL:
      {
         runner = new WindowsGameRunner( gameExePath );
         break;
      }

   case GDP_WINDOWS_DX9: // fallthrough
   default:
      {
         runner = new WindowsGameRunner( gameExePath );
         break;
      }
   }

   return runner;
}

///////////////////////////////////////////////////////////////////////////////
