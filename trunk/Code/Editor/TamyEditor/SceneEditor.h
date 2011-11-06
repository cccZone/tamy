#pragma once

/// @file   TamyEditor\SceneEditor.h
/// @brief  Simple editor for opening a new scene in the editor

#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class Model;
class TimeControllerTrack;
class QScrollArea;
class QPropertiesView;
class Entity;

///////////////////////////////////////////////////////////////////////////////

class SceneEditor : public ResourceEditor
{
   Q_OBJECT

private:
   Model&                     m_scene;

   // editor related stuff
   TimeControllerTrack*       m_sceneTrack;
   QIcon                      m_runSceneIcon;
   QIcon                      m_stopSceneIcon;
   QAction*                   m_actionRun;
   bool                       m_playing;
   QScrollArea*               m_scrollableDockWidgetContents;
   QPropertiesView*           m_selectedEntityView;

public:
   /**
    * Constructor.
    *
    * @param scene      scene to be edited
    */
   SceneEditor( Model& scene );

   // -------------------------------------------------------------------------
   // Selection sink
   // -------------------------------------------------------------------------
   void onEntitySelected( Entity& entity );
   void onEntityDeselected();

protected:
   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void onInitialize();

   // -------------------------------------------------------------------------
   // QFrame events
   // -------------------------------------------------------------------------
   void closeEvent( QCloseEvent *event );

private:
   void runScene();
   void stopScene();

public slots:
   void saveScene();
   void toggleSceneExecution();

};

///////////////////////////////////////////////////////////////////////////////
