#ifndef _TAMY_EDITOR_H
#define _TAMY_EDITOR_H

/// @file   TamyEditor\tamyeditor.h
/// @brief  editor's main window class

#include <QtGui/QMainWindow>
#include "ui_tamyeditor.h"
#include "core.h"
#include "EditorsDocker.h"


///////////////////////////////////////////////////////////////////////////////

class QApplication;
class QSettings;
class QDockWidget;
class QTreeWidget;
class TimeController;
class QTabWidget;

///////////////////////////////////////////////////////////////////////////////

/**
* This is the editor's main window class.
*/
class TamyEditor : public QMainWindow, public ComponentsManager< TamyEditor >, public EditorsDocker
{
   Q_OBJECT

private:
   static TamyEditor*               s_theInstance;

   Ui::TamyEditorClass              ui;
   QTabWidget*                      m_editorsTabs;

   // time tracking
   CTimer*                          m_mainTime;
   QTimer*                          m_mainTimeSlot;
   TimeController*                  m_timeController;

   // ui settings management
   QSettings*                       m_uiSettings;
   QSettings*                       m_editorSettings;

public:
   ~TamyEditor();

   // -------------------------------------------------------------------------
   // Singleton instance management
   // -------------------------------------------------------------------------
   /**
    * Creates a new singleton instance, destroying the old one if necessary.
    *
    * @param app        qt application running the show
    * @param fsRoot     file system root
    * @param parent     parent widget
    * @param flags      widget creation flags
    */
   static void createInstance( QApplication& app, const char* fsRoot, QWidget *parent = 0, Qt::WFlags flags = 0 );

   /**
    * Destroys the singleton instance.
    */
   static void destroyInstance();
   
   /**
    * Returns the singleton instance.
    */
   static TamyEditor& getInstance() { return *s_theInstance; }

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
   // EditorsDocker implementation
   // -------------------------------------------------------------------------
   void addEditor( ResourceEditor* editor );

public slots:
   void updateMain();
   void closeResourceEditor( int );

protected:
   void closeEvent( QCloseEvent *event );
   void serializeUISettings( bool save );

   void serializeWidgetSettings( QWidget& widget, bool save );
   void serializeDockWidgetSettings( QDockWidget& widget, bool save );
   void serializeTreeWidgetSettings( QTreeWidget& widget, bool save );

private:
   /**
    * Constructor.
    *
    * @param app        qt application running the show
    * @param fsRoot     file system root
    * @param parent     parent widget
    * @param flags      widget creation flags
    */
   TamyEditor( QApplication& app, const char* fsRoot, QWidget *parent = 0, Qt::WFlags flags = 0 );

   void setupResourcesManager( const char* fsRoot );
};

///////////////////////////////////////////////////////////////////////////////

#endif // _TAMY_EDITOR_H
