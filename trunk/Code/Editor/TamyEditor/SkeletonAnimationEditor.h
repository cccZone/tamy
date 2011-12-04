/// @file   TamyEditor/SkeletonAnimationEditor.h
/// @brief  skeleton animation resource editor
#pragma once

#include <QtGui/QMainWindow>
#include "ui_skeletonanimationeditor.h"
#include "ResourceEditor.h"
#include "core-AppFlow/TimeDependent.h"


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
class QSlider;
class QCheckBox;

///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimationEditor : public ResourceEditor, public TimeDependent
{
   Q_OBJECT

private:
   Ui::SkeletonAnimationEditorWindow         m_ui;

   const float                               TIME_SCALE;

   SkeletonAnimation&                        m_animation;
   Model*                                    m_scene;
   SkeletonAnimationKeysChart*               m_animationKeysChart;
   SkeletonAnimationEventsChart*             m_animationEventsChart;
   QListWidget*                              m_bonesList;
   TamySceneWidget*                          m_sceneWidget;
   TimeControllerTrack*                      m_timeTrack;

   ChartView*                                m_animationKeysChartView;
   VerticalChartMarker*                      m_keysTimeTrackingMarker;
   VerticalChartMarker*                      m_eventsTimeTrackingMarker;
   QSlider*                                  m_timeControlWidget;
   SkeletonAnimationController*              m_animationController;

   QCheckBox*                                m_posKeyXCheckBox;
   QCheckBox*                                m_posKeyYCheckBox;
   QCheckBox*                                m_posKeyZCheckBox;
   QCheckBox*                                m_orientKeyYawCheckBox;
   QCheckBox*                                m_orientKeyPitchCheckBox;
   QCheckBox*                                m_orientKeyRollCheckBox;

   // animation playback control panel
   QIcon                                     m_runSceneIcon;
   QIcon                                     m_stopSceneIcon;
   QAction*                                  m_actionPlay;
   bool                                      m_playing;

   std::vector< std::string >                m_animatedModelPaths;
   std::vector< std::string >                m_animationsPaths;

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
   void onDeinitialize( bool saveProgress );

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
   void animationImported();
   void onTimeValueChanged( int newValue );

private:
   void loadAnimatedModel( const std::string& modelResourcePath );
   void refresh();
};

///////////////////////////////////////////////////////////////////////////////
