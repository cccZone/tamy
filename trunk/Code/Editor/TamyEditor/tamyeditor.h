#pragma once

/// @file   TamyEditor\tamyeditor.h
/// @brief  editor's main window class

#include <QtGui/QMainWindow>
#include "ui_tamyeditor.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

class QApplication;
class Model;
class TimeController;
class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

/**
* This is the editor's main window class.
*/
class TamyEditor : public QMainWindow, public ComponentsManager<TamyEditor>
{
   Q_OBJECT

private:
   QApplication& m_app;
   Ui::TamyEditorClass ui;

   // time tracking
   CTimer* m_mainTime;
   QTimer* m_mainTimeSlot;
   TimeController* m_timeController;

   // scene the editor is operating on
   Model* m_scene;

   // resources manager
   ResourcesManager* m_resourceMgr;

public:
   /**
    * Constructor.
    *
    * @param app        qt application running the show
    * @param parent     parent widget
    * @param flags      widget creation flags
    */
   TamyEditor(QApplication& app, QWidget *parent = 0, Qt::WFlags flags = 0);
   ~TamyEditor();

   // -------------------------------------------------------------------------
   // window space management
   // -------------------------------------------------------------------------
   /**
    * Adds a new widget to the main window widget.
    *
    * @param widget     widget to add
    */
   void addToMainWidget(QWidget* widget);

   /**
    * Removes a widget from the main window widget.
    *
    * @param widget     widget to remove
    */
   void removeFromMainWidget(QWidget& widget);

   /**
    * This method gives access to the 'View' menu.
    * 
    * @return           view menu instance
    */
   QMenu& getViewMenu();

public slots:
   void updateMain();
   void loadScene();
   void saveScene();
   void importFromIWF();
};

///////////////////////////////////////////////////////////////////////////////
