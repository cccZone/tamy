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


///////////////////////////////////////////////////////////////////////////////

SkeletonAnimationEditor::SkeletonAnimationEditor( SkeletonAnimation& animation )
   : m_mgr( NULL )
   , m_animation( animation )
   , m_animationKeysChart( new SkeletonAnimationKeysChart( animation ) )
   , m_animationEventsChart( new SkeletonAnimationEventsChart( animation ) )
   , m_bonesList( NULL )
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

      QFrame* animationPreviewFrame = new QFrame( operationsSplitter );
      operationsSplitter->addWidget( animationPreviewFrame );

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
