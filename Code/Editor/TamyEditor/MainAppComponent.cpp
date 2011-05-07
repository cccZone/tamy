#include "MainAppComponent.h"
#include "core-AppFlow.h"
#include "core.h"
#include "ml-IWF.h"
#include "ml-Blender.h"
#include "ml-BVH.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QProgressBar.h>
#include <QMessageBox.h>
#include "SelectionManager.h"
#include "progressDialog.h"
#include "..\src\gui\dialogs\qfiledialog.h"


///////////////////////////////////////////////////////////////////////////////

MainAppComponent::MainAppComponent( QApplication& app, const char* fsRoot )
: m_app( app )
, m_timeController( new TimeController() )
, m_resourceMgr( &ResourcesManager::getInstance() )
, m_mgr( NULL )
{
   // add components
   Filesystem* fs = new Filesystem( fsRoot );
   fs->setShortcut( "editorIcons", "/Editor/Icons/" );
   m_resourceMgr->setFilesystem( fs );

   // register external resources
   m_resourceMgr->addLoader< BVHLoader >( "bvh" );
   m_resourceMgr->addLoader< IWFScene >( "iwf" );
   m_resourceMgr->addLoader< BlenderScene >( "dae" );
   m_resourceMgr->setProgressObserver< ProgressDialog >();

   // configure the scene execution track
   m_timeController->add( "scene" );
}

///////////////////////////////////////////////////////////////////////////////

MainAppComponent::~MainAppComponent()
{
   m_mgr = NULL;
   m_resourceMgr->reset(); m_resourceMgr = NULL;
   delete m_timeController; m_timeController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MainAppComponent::update( float timeElapsed )
{
   m_timeController->update( timeElapsed );
}

///////////////////////////////////////////////////////////////////////////////

void MainAppComponent::closeScene()
{
   if ( m_mgr )
   {
      // stop any scene that may be running
      stopScene();

      // delete the old scene
      Model* oldScene = NULL;
      if ( m_mgr->hasService< Model >() )
      {
         oldScene = &( m_mgr->requestService< Model >() );
         m_mgr->removeService< Model >( *this );

         // delete the old scene
         delete oldScene;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void MainAppComponent::setScene( Model& scene )
{
   if ( m_mgr )
   {
      // stop any scene that may be running
      stopScene();

      // we-re managing the scenes - so check if one's registered, and memorize
      // a pointer to it so that we can dispose of it afterwards
      Model* oldScene = NULL;
      if ( m_mgr->hasService< Model >() )
      {
         oldScene = &( m_mgr->requestService< Model >() );
      }

      // register the new scene
      m_mgr->registerService< Model >( *this, scene );

      // delete the old scene
      delete oldScene;
   }
}

///////////////////////////////////////////////////////////////////////////////

void MainAppComponent::toggleSceneExecution()
{
   TimeControllerTrack& sceneTrack = m_timeController->get( "scene" );
   if ( sceneTrack.getItemsCount() > 0 )
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

void MainAppComponent::stopScene()
{
   TimeControllerTrack& sceneTrack = m_timeController->get( "scene" );
   sceneTrack.reset();

   // update the UI
   m_actionRun->setIcon( m_runSceneIcon );
   m_actionRun->setText( "Run" );
}

///////////////////////////////////////////////////////////////////////////////

bool MainAppComponent::runScene()
{
   if ( !m_mgr )
   {
      return false;
   }

   if ( m_mgr->hasService< Model >() == false )
   {
      return false;
   }
  
   TimeControllerTrack& sceneTrack = m_timeController->get( "scene" );
   sceneTrack.reset();

   Model& scene = m_mgr->requestService< Model >();
   sceneTrack.add( new TTimeDependent< Model >( scene ) );

   // update the UI
   m_actionRun->setIcon( m_stopSceneIcon );
   m_actionRun->setText( "Stop" );

   return true;
}

///////////////////////////////////////////////////////////////////////////////

void MainAppComponent::initialize( TamyEditor& mgr )
{
   ASSERT_MSG( m_mgr == NULL, "MainAppComponent is already initialized" );
   m_mgr = &mgr;

   // setup services
   mgr.registerService< TimeController >( *this, *m_timeController );
   mgr.registerService< ResourcesManager >( *this, *m_resourceMgr );
   mgr.registerService< MainAppComponent >( *this, *this );

   // initialize user interface
   initUI( mgr );
}

///////////////////////////////////////////////////////////////////////////////

void MainAppComponent::initUI( TamyEditor& mgr )
{
   QString iconsDir = m_resourceMgr->getFilesystem().getShortcut( "editorIcons" ).c_str();
   QMenu& fileMenu = mgr.getFileMenu();
   QToolBar& toolBar = mgr.getToolBar();

   // setup file menu entries
   {
      QAction* actionNewScene = new QAction( QIcon( iconsDir + tr( "/world.png" ) ), tr( "New Scene" ), &mgr );
      fileMenu.addAction( actionNewScene );
      toolBar.addAction( actionNewScene );
      connect( actionNewScene, SIGNAL( triggered() ), this, SLOT( newScene() ) );

      QAction* actionLoadScene = new QAction( QIcon( iconsDir + tr( "/openFile.png" ) ), tr( "Load Scene" ), &mgr );
      fileMenu.addAction( actionLoadScene );
      toolBar.addAction( actionLoadScene );
      connect( actionLoadScene, SIGNAL( triggered() ), this, SLOT( loadScene() ) );

      QAction* actionSaveScene = new QAction( QIcon( iconsDir + tr( "/saveFile.png" ) ), tr( "Save Scene" ), &mgr );
      fileMenu.addAction( actionSaveScene );
      toolBar.addAction( actionSaveScene );
      connect( actionSaveScene, SIGNAL( triggered() ), this, SLOT( saveScene() ) );

      toolBar.addSeparator();

      QAction* separator1 = new QAction( &fileMenu );
      separator1->setSeparator( true );
      fileMenu.addAction( separator1 );
      
      QAction* separator2 = new QAction( &fileMenu );
      separator2->setSeparator( true );
      fileMenu.addAction( separator2 );

      QAction* actionQuit = new QAction( QIcon( iconsDir + tr( "/quit.png" ) ), tr( "Quit" ), &mgr );
      fileMenu.addAction( actionQuit );
      connect( actionQuit, SIGNAL( triggered() ), &m_app, SLOT( quit() ) );
   }

   //setup execution commands
   {
      m_runSceneIcon = QIcon( iconsDir + tr( "/play.png" ) );
      m_stopSceneIcon = QIcon( iconsDir + tr( "/stop.png" ) );

      m_actionRun = new QAction( m_runSceneIcon, tr( "Run" ), &mgr );
      m_actionRun->setShortcut( QKeySequence( tr( "F5" ) ) );
      toolBar.addAction( m_actionRun );
      connect( m_actionRun, SIGNAL( triggered() ), this, SLOT( toggleSceneExecution() ) );

      toolBar.addSeparator();
   }
}

///////////////////////////////////////////////////////////////////////////////

void MainAppComponent::newScene()
{
   const Filesystem& fs = m_resourceMgr->getFilesystem();
   std::string rootDir = fs.getCurrRoot();
   std::string filter( "Scene files (*." );
   filter += std::string( Model::getExtension() ) + ")";

   QString fullFileName = QFileDialog::getSaveFileName( m_mgr, 
      tr("New scene"), 
      rootDir.c_str(), 
      filter.c_str() );

   if ( fullFileName.isEmpty() == true ) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }

   // once the file is open, extract the directory name
   std::string fileName = fs.toRelativePath( fullFileName.toStdString() );

   if ( fs.doesExist( fileName ) )
   {
      QMessageBox::warning( m_mgr , "New scene creation",
         QString( "Scene " ) + fileName.c_str() + "already exists - pick a different name",
         QMessageBox::Ok );
      return;
   }

   try
   {
      Model* newScene = new Model( fileName );
      m_resourceMgr->addResource( newScene );
      newScene->saveResource();

      setScene( *newScene );
   }
   catch (std::exception& ex)
   {
      QMessageBox::warning( m_mgr, QString("Error occurred while creating scene ") + fileName.c_str(), ex.what(), QMessageBox::Ok );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MainAppComponent::loadScene()
{
   const Filesystem& fs = m_resourceMgr->getFilesystem();
   std::string rootDir = fs.getCurrRoot();
   std::string filter( "Scene files (*." );
   filter += std::string( Model::getExtension() ) + ")";

   QString fullFileName = QFileDialog::getOpenFileName( m_mgr, 
      tr("Load scene"), 
      rootDir.c_str(), 
      filter.c_str() );

   if ( fullFileName.isEmpty() == true ) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }

   // once the file is open, extract the directory name
   std::string fileName = fs.toRelativePath( fullFileName.toStdString() );

   try
   {
      // close the old scene
      closeScene();

      ProgressDialog progressObserver( m_mgr );
      progressObserver.initialize( "Loading a scene", 1 );
      Model& newScene = dynamic_cast< Model& >( m_resourceMgr->create( fileName ) );
      progressObserver.advance();

      setScene( newScene );
   }
   catch (std::exception& ex)
   {
      QMessageBox::warning( m_mgr, QString("Error occurred while loading scene ") + fileName.c_str(), ex.what(), QMessageBox::Ok );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MainAppComponent::saveScene()
{
   Model& scene = m_mgr->requestService< Model >();

   try
   {
      ProgressDialog progressObserver( m_mgr );
      progressObserver.initialize( "Mapping the dependencies", 1 );
      ExternalDependenciesSet externalDependencies;
      scene.saveResource( externalDependencies );

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
      QMessageBox::warning( m_mgr, QString("Error occurred while saving scene"), ex.what(), QMessageBox::Ok );

      return;
   }
}

///////////////////////////////////////////////////////////////////////////////
