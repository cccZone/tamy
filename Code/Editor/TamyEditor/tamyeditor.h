#ifndef _TAMY_EDITOR_H
#define _TAMY_EDITOR_H

/// @file   TamyEditor\tamyeditor.h
/// @brief  editor's main window class

#include <QtGui/QMainWindow>
#include "ui_tamyeditor.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

class QApplication;
class QSettings;
class QDockWidget;
class QTreeWidget;
class TimeController;

///////////////////////////////////////////////////////////////////////////////

/**
* This is the editor's main window class.
*/
class TamyEditor : public QMainWindow, public ComponentsManager< TamyEditor >
{
   Q_OBJECT

private:
   Ui::TamyEditorClass              ui;

   // time tracking
   CTimer*                          m_mainTime;
   QTimer*                          m_mainTimeSlot;
   TimeController*                  m_timeController;

   // ui settings management
   QSettings*                       m_uiSettings;
   QSettings*                       m_editorSettings;

   std::vector< QMainWindow* >      m_subEditors;

public:
   /**
    * Constructor.
    *
    * @param app        qt application running the show
    * @param fsRoot     file system root
    * @param parent     parent widget
    * @param flags      widget creation flags
    */
   TamyEditor( QApplication& app, const char* fsRoot, QWidget *parent = 0, Qt::WFlags flags = 0 );
   ~TamyEditor();

   // -------------------------------------------------------------------------
   // window space management
   // -------------------------------------------------------------------------
   /**
    * Adds a new widget to the main window widget.
    *
    * @param widget     widget to add
    */
   void addToMainWidget( QWidget* widget );

   /**
    * Removes a widget from the main window widget.
    *
    * @param widget     widget to remove
    */
   void removeFromMainWidget( QWidget& widget );

   /**
    * This method gives access to the 'File' menu.
    * 
    * @return           file menu instance
    */
   QMenu& getFileMenu();

   /**
    * This method gives access to the 'View' menu.
    * 
    * @return           view menu instance
    */
   QMenu& getViewMenu();

   /**
    * This method gives access to the main toolbar.
    * 
    * @return           toolbar instance
    */
   QToolBar& getToolBar();

   /**
    * Returns editor related settings.
    */
   inline QSettings& getSettings() { return *m_editorSettings; }

   /**
    * Returns the time controller instance.
    */
   inline TimeController& getTimeController() const { return *m_timeController; }

   // -------------------------------------------------------------------------
   // Sub editor windows management
   // -------------------------------------------------------------------------
   /**
    * Puts a sub editor up for management.
    *
    * @param editor
    */
   void registerSubEditor( QMainWindow* editor );

   /**
    * Umanages a sub editor.
    *
    * @param editor
    */
   void unregisterSubEditor( QMainWindow& editor );

public slots:
   void updateMain();

protected:
   void closeEvent( QCloseEvent *event );
   void serializeUISettings( bool save );

   void serializeWidgetSettings( QWidget& widget, bool save );
   void serializeDockWidgetSettings( QDockWidget& widget, bool save );
   void serializeTreeWidgetSettings( QTreeWidget& widget, bool save );
};

///////////////////////////////////////////////////////////////////////////////

extern TamyEditor* GTamyEditor;

///////////////////////////////////////////////////////////////////////////////

#endif // _TAMY_EDITOR_H
