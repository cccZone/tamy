/// @file   TamyEditor/SkeletonAnimationEditor.h
/// @brief  skeleton animation resource editor
#pragma once

#include <QtGui/QMainWindow>
#include "ui_skeletonanimationeditor.h"
#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimation;
class SkeletonAnimationController;
class SkeletonAnimationKeysChart;
class SkeletonAnimationEventsChart;
class QListWidget;
class QListWidgetItem;
class TamySceneWidget;
class TimeControllerTrack;
class Model;
class QSplitter;
class ChartView;
class ChartScene;
class VerticalChartMarker;

///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimationEditor : public ResourceEditor
{
   Q_OBJECT

private:
   Ui::SkeletonAnimationEditorWindow         m_ui;

   SkeletonAnimation&                        m_animation;
   Model*                                    m_scene;
   SkeletonAnimationKeysChart*               m_animationKeysChart;
   SkeletonAnimationEventsChart*             m_animationEventsChart;
   QListWidget*                              m_bonesList;
   TamySceneWidget*                          m_sceneWidget;
   TimeControllerTrack*                      m_timeTrack;

   ChartView*                                m_animationKeysChartView;
   VerticalChartMarker*                      m_keysTimeTrackingMarker;
   ChartView*                                m_animationEventsChartView;
   VerticalChartMarker*                      m_eventsTimeTrackingMarker;
   QSlider*                                  m_timeControlWidget;
   SkeletonAnimationController*              m_animationController;

   // animation playback control panel
   QIcon                                     m_runSceneIcon;
   QIcon                                     m_stopSceneIcon;
   QAction*                                  m_actionPlay;
   bool                                      m_playing;

   std::vector< std::string >                m_animatedModelPaths;

public:
   /**
    * Constructor.
    *
    * @param animation     edited animation
    */
   SkeletonAnimationEditor( SkeletonAnimation& animation );
   ~SkeletonAnimationEditor();

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void onInitialize();

   // -------------------------------------------------------------------------
   // TimeDependent implementation
   // -------------------------------------------------------------------------
   void update( float timeElapsed );

public slots:
   void onBoneSelected( QListWidgetItem* item );
   void onTogglePosKeyX( int state );
   void onTogglePosKeyY( int state );
   void onTogglePosKeyZ( int state );
   void onToggleOrientKeyYaw( int state );
   void onToggleOrientKeyPitch( int state );
   void onToggleOrientKeyRoll( int state );
   void togglePlay();
   void animatedModelLoaded();

protected:
   void closeEvent( QCloseEvent *event );

private:
   void loadAnimatedModel( const std::string& modelResourcePath );
};

///////////////////////////////////////////////////////////////////////////////
