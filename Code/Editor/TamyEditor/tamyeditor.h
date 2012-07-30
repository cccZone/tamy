/// @file   TamyEditor\tamyeditor.h
/// @brief  editor's main window class
#ifndef _TAMY_EDITOR_H
#define _TAMY_EDITOR_H

#include <QtGui/QMainWindow>
#include "ui_tamyeditor.h"
#include "core.h"
#include "core\GenericFactory.h"
#include "ResourceEditor.h"
#include "core/Delegate.h"
#include "core/FilePath.h"


///////////////////////////////////////////////////////////////////////////////

class QApplication;
class QSettings;
class QDockWidget;
class QTreeWidget;
class TimeController;
class MainEditorPanel;
class ResourcesBrowser;
class Project;
class ProfilerFrame;

///////////////////////////////////////////////////////////////////////////////

/**
* This is the editor's main window class.
*/
class TamyEditor : public QMainWindow, public FilesystemListener, public GenericFactory< Resource, ResourceEditor >
{
   Q_OBJECT

private:
   typedef Delegate< bool ( const FilePath&, const FilePath& ) >     PathTest;

private:
   static TamyEditor*               s_theInstance;

   QApplication&                    m_app;
   Ui::TamyEditorClass              ui;
   MainEditorPanel*                 m_editorsTabs;

   // time tracking
   CTimer*                          m_mainTime;
   QTimer*                          m_mainTimeSlot;
   TimeController*                  m_timeController;

   // ui settings management
   QSettings*                       m_editorSettings;

   // resources browser
   ResourcesBrowser*                m_resourcesBrowser;

   // project management
   Project*                         m_activeProject;
   // we need this in case the project gets deleted - the resource may have already been deleted, and the only way to tell 
   // if the deleted project was the active one would be to compare the paths
   FilePath                         m_activeProjectPath;

   // profiler view
   ProfilerFrame*                   m_activeProfilerView;

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

   /**
    * Returns the resources browser instance.
    */
   inline ResourcesBrowser& getResourcesBrowser() { return *m_resourcesBrowser; }

   // -------------------------------------------------------------------------
   // window space management
   // -------------------------------------------------------------------------

   /**
    * Sets the currently active tab widget.
    *
    * @param tabWidget
    */
   void setActiveTabWidget( QTabWidget* tabWidget );

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
   // Resources edition
   // -------------------------------------------------------------------------
   /**
    * Creates a resources editor for the specified resource
    *
    * @param resource
    * @param icon
    */
   ResourceEditor* createResourceEditor( Resource* resource, const QIcon& icon );

   /**
    * Adds the specified editor to the main window.
    *
    * @param editor
    * @param dock       should the editor be added to the editors tab manager ( Qt::NoDockWidgetArea ), or docked in a separate window
    */
   void addResourceEditor( ResourceEditor* editor, Qt::DockWidgetArea dockArea = Qt::NoDockWidgetArea );

   /**
    * Finds and activates a resource editor corresponding to the specified resource.
    *
    * @param resource
    * @return           'true' if an editor exists and we were able to activate it, 'false' otherwise
    */
   bool activateResourceEditor( Resource* resource );

   // -------------------------------------------------------------------------
   // Project management
   // -------------------------------------------------------------------------
   /**
    * Sets the specified project as an active one
    */
   void setActiveProject( Project* project );

   /**
    * Returns the active project instance, or NULL if no project is active at the moment.
    */
   inline Project* getActiveProject() const { return m_activeProject; }

   // -------------------------------------------------------------------------
   // FilesystemListener implementation
   // -------------------------------------------------------------------------
   void onDirAdded( const FilePath& dir );
   void onDirRemoved( const FilePath& dir );
   void onFileEdited( const FilePath& path );
   void onFileRemoved( const FilePath& path );

public slots:
   void onEditorTabClosed( QWidget* editorWidget );
   void updateMain();
   void startProfiler();

protected:
   void closeEvent( QCloseEvent *event );

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

   /**
    * Closes all active editors that pass the specified path test.
    *
    * @param path
    * @param test
    */
   void closeEditors( const FilePath& path, const PathTest& test );

   // -------------------------------------------------------------------------
   // path tests
   // -------------------------------------------------------------------------
   bool isSamePath( const FilePath& resourcePath, const FilePath& checkedPath );
   bool isParentDir( const FilePath& resourcePath, const FilePath& dir );
   bool isResourceFromDeletedDir( const FilePath& resourcePath, const FilePath& deletedDir );
   bool allTrue( const FilePath& resourcePath, const FilePath& deletedDir ) { return true; }
};

///////////////////////////////////////////////////////////////////////////////

#endif // _TAMY_EDITOR_H
