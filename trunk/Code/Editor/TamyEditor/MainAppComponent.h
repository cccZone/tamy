#pragma once

/// @file   TamyEditor/MainAppComponent.h
/// @brief  main editor application component

#include "core/Component.h"
#include "tamyeditor.h"


///////////////////////////////////////////////////////////////////////////////

class TimeController;
class ResourcesManager;
class Model;

///////////////////////////////////////////////////////////////////////////////

class MainAppComponent : public QObject, public Component< TamyEditor >
{
   Q_OBJECT

private:
   QApplication&        m_app;

   TimeController*      m_timeController;
   ResourcesManager*    m_resourceMgr;

   TamyEditor*          m_mgr;

   // editor related stuff
   QIcon                m_runSceneIcon;
   QIcon                m_stopSceneIcon;
   QAction*             m_actionRun;

public:
   /**
    * Contructor.
    *
    * @param app     instance of QApplication running this component
    * @param fsRoot  filesystem root
    */
   MainAppComponent( QApplication& app, const char* fsRoot );
   ~MainAppComponent();

   // -------------------------------------------------------------------------
   // Scene management
   // -------------------------------------------------------------------------
   /**
    * Sets a new scene.
    *
    * @param scene
    */
   void setScene( Model& scene );

   /**
    * Closes the active scene.
    */
   void closeScene();

   // -------------------------------------------------------------------------
   // Component initialization
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );

public slots:
   void newScene();
   void loadScene();
   void saveScene();
   void toggleSceneExecution();

private:
   void initUI( TamyEditor& mgr );

   /**
    * Runs a scene.
    */
   bool runScene();

   /**
    * Stops a running scene.
    */
   void stopScene();
};

///////////////////////////////////////////////////////////////////////////////
