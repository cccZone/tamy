/// @file   TamyEditor/SkeletonAnimationEditor.h
/// @brief  skeleton animation resource editor
#pragma once

#include <QtGui/QMainWindow>
#include "ui_skeletonanimationeditor.h"
#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimation;
class SkeletonAnimationKeysChart;
class SkeletonAnimationEventsChart;
class QListWidget;
class QListWidgetItem;
class TamySceneWidget;
class TimeControllerTrack;
class Model;

///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimationEditor : public QMainWindow, public ResourceEditor
{
   Q_OBJECT

private:
   Ui::SkeletonAnimationEditorWindow         m_ui;

   TamyEditor*                               m_mgr;
   SkeletonAnimation&                        m_animation;
   Model*                                    m_scene;
   SkeletonAnimationKeysChart*               m_animationKeysChart;
   SkeletonAnimationEventsChart*             m_animationEventsChart;
   QListWidget*                              m_bonesList;
   TamySceneWidget*                          m_sceneWidget;
   TimeControllerTrack*                      m_timeTrack;

   // animation playback control panel
   QIcon                                     m_runSceneIcon;
   QIcon                                     m_stopSceneIcon;
   QAction*                                  m_actionPlay;
   bool                                      m_playing;

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
   void initialize( TamyEditor& mgr );

public slots:
   void onBoneSelected( QListWidgetItem* item );
   void onTogglePosKeyX( int state );
   void onTogglePosKeyY( int state );
   void onTogglePosKeyZ( int state );
   void onToggleOrientKeyYaw( int state );
   void onToggleOrientKeyPitch( int state );
   void onToggleOrientKeyRoll( int state );
   void togglePlay();

protected:
   void closeEvent( QCloseEvent *event );

private:
   bool visualizeAnimation();
};

///////////////////////////////////////////////////////////////////////////////
