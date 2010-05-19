#ifndef _TAMY_EDITOR_H
#define _TAMY_EDITOR_H

/// @file   TamyEditor\tamyeditor.h
/// @brief  editor's main window class

#include <QtGui/QMainWindow>
#include "ui_tamyeditor.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

class QApplication;
class MainAppComponent;
class QSettings;
class QDockWidget;
class QTreeWidget;

///////////////////////////////////////////////////////////////////////////////

/**
* This is the editor's main window class.
*/
class TamyEditor : public QMainWindow, public ComponentsManager<TamyEditor>
{
   Q_OBJECT

private:
   Ui::TamyEditorClass        ui;

   // time tracking
   CTimer*                    m_mainTime;
   QTimer*                    m_mainTimeSlot;

   MainAppComponent*          m_mainAppComponent;

   // ui settings management
   QSettings*                 m_uiSettings;

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

#endif // _TAMY_EDITOR_H
