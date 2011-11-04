#include "SkeletonAnimationEditor.h"
#include "tamyeditor.h"
#include <QCloseEvent>
#include <QSplitter>
#include <QGraphicsView>
#include <QListWidget>
#include <QCheckBox>
#include <QScrollArea>
#include "SkeletonAnimationKeysChart.h"
#include "SkeletonAnimationEventsChart.h"
#include "core-AI/SkeletonAnimation.h"
#include "core-AI/SkeletonAnimationController.h"
#include "core-MVC/ModelEntity.h"
#include "core-MVC/Model.h"
#include "core-AppFlow.h"
#include "core-Renderer/Skeleton.h"
#include "TamySceneWidget.h"
#include <QSettings>
#include <QFileDialog>


///////////////////////////////////////////////////////////////////////////////

SkeletonAnimationEditor::SkeletonAnimationEditor( SkeletonAnimation& animation )
   : m_mgr( NULL )
   , m_animation( animation )
   , m_animationKeysChart( new SkeletonAnimationKeysChart( animation ) )
   , m_animationEventsChart( new SkeletonAnimationEventsChart( animation ) )
   , m_bonesList( NULL )
   , m_scene( NULL )
   , m_timeTrack( NULL )
   , m_playing( false )
{
   setAttribute( Qt::WA_DeleteOnClose );
}

///////////////////////////////////////////////////////////////////////////////

SkeletonAnimationEditor::~SkeletonAnimationEditor()
{
   if ( m_mgr )
   {
      // unregister the sub editor
      m_mgr->unregisterSubEditor( *this );
   }

   if ( m_scene )
   {
      m_timeTrack->remove( *m_scene );
   }
   m_timeTrack = NULL;

   delete m_scene;
   m_scene = NULL;

   delete m_animationKeysChart;
   m_animationKeysChart = NULL;

   delete m_animationEventsChart;
   m_animationEventsChart = NULL;

   m_bonesList = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::initialize( TamyEditor& mgr )
{
   // register the sub editor with the main editor
   m_mgr = &mgr;
   m_mgr->registerSubEditor( this );

   TimeController& controller = mgr.getTimeController();
   m_timeTrack = &controller.add( "animationEditor" );

   ResourcesManager& resMgr = ResourcesManager::getInstance();
   QString iconsDir = resMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();

   // setup ui
   m_ui.setupUi( this );

   // attach the graph viewer
   QSplitter* mainSplitter = new QSplitter( m_ui.chartsFrame );
   m_ui.chartsFrameLayout->addWidget( mainSplitter );
   mainSplitter->setOrientation( Qt::Horizontal );

   // left side frames
   {
      QSplitter* operationsSplitter = new QSplitter( mainSplitter );
      mainSplitter->addWidget( operationsSplitter );
      operationsSplitter->setOrientation( Qt::Vertical );

      TimeController& timeController = mgr.requestService< TimeController >();
      QSettings& settings = mgr.getSettings();
      settings.beginGroup( "MainRenderer" );
      std::string mainRendererPipelineName = settings.value( "pipeline", "" ).toString().toStdString();
      settings.endGroup();

      // create the preview frame
      QFrame* animationPreviewFrame = new QFrame( operationsSplitter );
      operationsSplitter->addWidget( animationPreviewFrame );
      QVBoxLayout* previewFrameMasterLayout = new QVBoxLayout();
      animationPreviewFrame->setLayout( previewFrameMasterLayout );
      animationPreviewFrame->resize( 200, 200 );

      {
         // create the scene renderer
         QFrame* renderViewportFrame = new QFrame( animationPreviewFrame );
         previewFrameMasterLayout->addWidget( renderViewportFrame, 1 );
         QLayout* renderViewportLayout = new QHBoxLayout();
         renderViewportFrame->setLayout( renderViewportLayout );
         renderViewportLayout->setMargin( 0 );
         renderViewportLayout->setSpacing( 0 );
         renderViewportFrame->setFrameStyle( QFrame::Panel | QFrame::Sunken );
         renderViewportFrame->setLineWidth( 2 );

         m_sceneWidget = new TamySceneWidget( renderViewportFrame, 0, mainRendererPipelineName, timeController );
         renderViewportLayout->addWidget( m_sceneWidget );

         // add a control panel so that we can play the scene
         QToolBar* playbackControlPanel = new QToolBar( animationPreviewFrame );
         previewFrameMasterLayout->addWidget( playbackControlPanel, 0 );

         m_runSceneIcon = QIcon( iconsDir + tr( "/play.png" ) );
         m_stopSceneIcon = QIcon( iconsDir + tr( "/stop.png" ) );

         m_actionPlay = new QAction( m_runSceneIcon, tr( "Run" ), playbackControlPanel );
         playbackControlPanel->addAction( m_actionPlay );
         connect( m_actionPlay, SIGNAL( triggered() ), this, SLOT( togglePlay() ) );
      }


      // TODO: camera controller

      m_bonesList = new QListWidget( operationsSplitter );
      operationsSplitter->addWidget( m_bonesList );
      connect( m_bonesList, SIGNAL( itemClicked( QListWidgetItem* ) ), this, SLOT( onBoneSelected( QListWidgetItem* ) ) );

      std::vector< std::string > bonesNames;
      m_animation.collectBonesNames( bonesNames );
      for ( std::vector< std::string >::const_iterator it = bonesNames.begin(); it != bonesNames.end(); ++it )
      {
         m_bonesList->addItem( it->c_str() );
      }
   }

   // right side frames
   {
      QSplitter* chartsSplitter = new QSplitter( mainSplitter );
      mainSplitter->addWidget( chartsSplitter );
      chartsSplitter->setOrientation( Qt::Vertical );

      // animation keys chart
      {
         QFrame* animationKeysChartFrame = new QFrame( chartsSplitter );
         chartsSplitter->addWidget( animationKeysChartFrame );

         QHBoxLayout* animationKeysChartFrameLayout = new QHBoxLayout( animationKeysChartFrame );
         animationKeysChartFrame->setLayout( animationKeysChartFrameLayout );

         // keys selection frame
         {
            QFrame* keysSelectionFrame = new QFrame( animationKeysChartFrame );
            animationKeysChartFrameLayout->addWidget( keysSelectionFrame );

            QVBoxLayout* keysSelectionLayout = new QVBoxLayout( keysSelectionFrame );
            keysSelectionFrame->setLayout( keysSelectionLayout );

            QCheckBox* posKeyXCheckBox = new QCheckBox( "X", keysSelectionFrame );
            keysSelectionLayout->addWidget( posKeyXCheckBox );
            connect( posKeyXCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( onTogglePosKeyX( int ) ) );

            QCheckBox* posKeyYCheckBox = new QCheckBox( "Y", keysSelectionFrame );
            keysSelectionLayout->addWidget( posKeyYCheckBox );
            connect( posKeyYCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( onTogglePosKeyY( int ) ) );

            QCheckBox* posKeyZCheckBox = new QCheckBox( "Z", keysSelectionFrame );
            keysSelectionLayout->addWidget( posKeyZCheckBox );
            connect( posKeyZCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( onTogglePosKeyZ( int ) ) );

            QCheckBox* orientKeyYawCheckBox = new QCheckBox( "Yaw", keysSelectionFrame );
            keysSelectionLayout->addWidget( orientKeyYawCheckBox );
            connect( orientKeyYawCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( onToggleOrientKeyYaw( int ) ) );

            QCheckBox* orientKeyPitchCheckBox = new QCheckBox( "Pitch", keysSelectionFrame );
            keysSelectionLayout->addWidget( orientKeyPitchCheckBox );
            connect( orientKeyPitchCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( onToggleOrientKeyPitch( int ) ) );

            QCheckBox* orientKeyRollCheckBox = new QCheckBox( "Roll", keysSelectionFrame );
            keysSelectionLayout->addWidget( orientKeyRollCheckBox );
            connect( orientKeyRollCheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( onToggleOrientKeyRoll( int ) ) );
         }

         QGraphicsView* animationKeysChartView = new QGraphicsView( m_animationKeysChart, chartsSplitter );
         animationKeysChartFrameLayout->addWidget( animationKeysChartView );
         animationKeysChartView->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
      }

      QGraphicsView* animationEventsChartView = new QGraphicsView( m_animationEventsChart, chartsSplitter );
      chartsSplitter->addWidget( animationEventsChartView );
      animationEventsChartView->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
   }

   // adjust the main splitter size
   QList< int > sizes;
   sizes.push_back( 150 );
   sizes.push_back( 1000 );
   mainSplitter->setSizes( sizes );

   // show the editor
   show();
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::closeEvent( QCloseEvent *event )
{
   // accept the event
   event->accept();

   delete this;
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::onBoneSelected( QListWidgetItem* item )
{
   std::string boneName = item->text().toStdString();
   m_animationKeysChart->toggleBone( boneName );
   if ( m_animationKeysChart->isBoneDisplayed( boneName ) )
   {
      item->setTextColor( QColor( 204, 170, 0 ) );
   }
   else
   {
      item->setTextColor( QColor( 0, 0, 0 ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::onTogglePosKeyX( int state )
{
   m_animationKeysChart->toggleKeyVisibility( BAKEY_POS_X );
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::onTogglePosKeyY( int state )
{
   m_animationKeysChart->toggleKeyVisibility( BAKEY_POS_Y );
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::onTogglePosKeyZ( int state )
{
   m_animationKeysChart->toggleKeyVisibility( BAKEY_POS_Z );
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::onToggleOrientKeyYaw( int state )
{
   m_animationKeysChart->toggleKeyVisibility( BAKEY_YAW );
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::onToggleOrientKeyPitch( int state )
{
   m_animationKeysChart->toggleKeyVisibility( BAKEY_PITCH );
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::onToggleOrientKeyRoll( int state )
{
   m_animationKeysChart->toggleKeyVisibility( BAKEY_ROLL );
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEditor::togglePlay()
{
   if ( m_scene == NULL && !visualizeAnimation() )
   {
      // still no scene - break it
      return;
   }

   if ( m_playing )
   {
      m_timeTrack->remove( *m_scene );

      // update the UI
      m_actionPlay->setIcon( m_runSceneIcon );
      m_actionPlay->setText( "Run" );
   }
   else
   {
      m_timeTrack->add( *m_scene );

      // update the UI
      m_actionPlay->setIcon( m_stopSceneIcon );
      m_actionPlay->setText( "Stop" );
   }
   m_playing = !m_playing;
}

///////////////////////////////////////////////////////////////////////////////

bool SkeletonAnimationEditor::visualizeAnimation()
{
   // TODO: ability to drag and drop a model resource from the resources browser

   // destroy the previous scene
   if ( m_scene )
   {
      m_sceneWidget->clearScene();
   }
   delete m_scene;

   // no scene with an animation - try creating one, but you're gonna need a skeleton first
   const Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   std::string rootDir = fs.getCurrRoot();
   std::string filter( "Model files (*." );
   filter += std::string( Model::getExtension() ) + ")";

   QString fullFileName = QFileDialog::getOpenFileName( m_mgr, tr("Load model"), rootDir.c_str(), filter.c_str() );

   if ( fullFileName.isEmpty() == true ) 
   {
      // no file was selected or user pressed 'cancel'
      return false;
   }

   // once the file is open, extract the directory name
   std::string modelResourcePath = fs.toRelativePath( fullFileName.toStdString() );

   // load the skeleton
   Model* modelForAnimation = DynamicCast< Model >( &ResourcesManager::getInstance().create( modelResourcePath ) );
   if ( !modelForAnimation )
   {
      // couldn't load the model resource
      return false;
   }

   m_scene = new Model();
   m_scene->add( new ModelEntity( *modelForAnimation, "animatedModel" ) );

   Entity* skeletonRootEntity = m_scene->findFirstEntity( "Hips" );
   if ( !skeletonRootEntity )
   {
      // no root bone
      return false;
   }

   // create a scene using the animation
   SkeletonAnimationController* animationController = new SkeletonAnimationController( "animation" );
   animationController->setAnimationSource( m_animation );
   skeletonRootEntity->add( animationController );

   m_sceneWidget->setScene( *m_scene );

   return true;
}

///////////////////////////////////////////////////////////////////////////////

// TODO: one of the bones is drawn invalidly - learn why
// TODO: visualisation of the time position and ability to change it by hand