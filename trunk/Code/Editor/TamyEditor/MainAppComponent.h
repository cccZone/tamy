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

public:
   /**
    * Contructor.
    *
    * @param app     instance of QApplication running this component
    */
   MainAppComponent( QApplication& app );
   ~MainAppComponent();

   /**
    * Updates the state of the editor.
    */
   void update( float timeElapsed );

   // -------------------------------------------------------------------------
   // Scene management
   // -------------------------------------------------------------------------
   /**
    * Sets a new scene.
    *
    * @param scene
    */
   void setScene( Model& scene );

   // -------------------------------------------------------------------------
   // Component initialization
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );

public slots:
   void newScene();
   void loadScene();
   void saveScene();
   void importFromIWF();
   void importFromBlender();

private:
   void initUI( TamyEditor& mgr );
};

///////////////////////////////////////////////////////////////////////////////
