#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "core-Renderer.h"
#include "core-MVC.h"
#include "progressdialog.h"
#include <QTimer.h>
#include <QSettings.h>
#include <QCloseEvent>
#include <QDockWidget.h>
#include <QTreeWidget.h>


// components
#include "MainAppComponent.h"
#include "SelectionManager.h"
#include "TamySceneWidget.h"
#include "SceneNavigator.h"
#include "SceneQueries.h"
#include "PropertiesEditor.h"
#include "SceneTreeViewer.h"
#include "ResourcesBrowser.h"


///////////////////////////////////////////////////////////////////////////////

TamyEditor* GTamyEditor = NULL;

///////////////////////////////////////////////////////////////////////////////

TamyEditor::TamyEditor( QApplication& app, const char* fsRoot, QWidget *parent, Qt::WFlags flags )
: QMainWindow( parent, flags )
, m_mainTime( new CTimer() )
, m_mainAppComponent( new MainAppComponent( app, fsRoot ) )
{
   ui.setupUi(this);

   m_mainTimeSlot = new QTimer(this);
   connect(m_mainTimeSlot, SIGNAL(timeout()), this, SLOT(updateMain()));
   m_mainTimeSlot->start(1);

   // load the settings file
   m_editorSettings = new QSettings( ( std::string( fsRoot ) + "/Editor/Settings.ini" ).c_str(), QSettings::IniFormat );

   // add components
   addComponent( m_mainAppComponent );
   addComponent( new TamySceneWidget( ui.renderWindow, 0 ) );
   addComponent( new SelectionManager() );
   addComponent( new SceneNavigator() );
   addComponent( new SceneQueries() );
   addComponent( new PropertiesEditor() );
   addComponent( new SceneTreeViewer() );
   addComponent( new ResourcesBrowser() );

   m_uiSettings = new QSettings( "Coversion", "TamyEditor" );
   serializeUISettings( false );

   // initialize the singleton instance
   GTamyEditor = this;
}

///////////////////////////////////////////////////////////////////////////////

TamyEditor::~TamyEditor()
{
   m_mainAppComponent = NULL;
   delete m_mainTime; m_mainTime = NULL;
   delete m_mainTimeSlot; m_mainTimeSlot = NULL;
   delete m_uiSettings; m_uiSettings = NULL;
   delete m_editorSettings; m_editorSettings = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::addToMainWidget(QWidget* widget)
{
   ui.renderWindow->layout()->addWidget( widget );
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::removeFromMainWidget(QWidget& widget)
{
   ui.renderWindow->layout()->removeWidget( &widget );
}

///////////////////////////////////////////////////////////////////////////////

QMenu& TamyEditor::getFileMenu()
{
   return *(ui.menuFile);
}

///////////////////////////////////////////////////////////////////////////////

QMenu& TamyEditor::getViewMenu()
{
   return *(ui.menuView);
}

///////////////////////////////////////////////////////////////////////////////

QToolBar& TamyEditor::getToolBar()
{
   return *(ui.mainToolBar);
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::updateMain()
{
   m_mainTime->tick();
   float timeElapsed = m_mainTime->getTimeElapsed();

   m_mainAppComponent->update( timeElapsed );
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::closeEvent( QCloseEvent *event )
{
   // close all active sub editors
   std::vector< QMainWindow* > editorsToClose = m_subEditors;
   m_subEditors.clear();
   for ( std::vector< QMainWindow* >::iterator it = editorsToClose.begin(); it != editorsToClose.end(); ++it )
   {
      (*it)->close();
   }

   // serialize the settings
   serializeUISettings( true );

   // accept the event
   event->accept();
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::serializeUISettings( bool save )
{
   std::list< QWidget* >   widgetsQueue;
   widgetsQueue.push_back( this );

   while( !widgetsQueue.empty() )
   {
      QWidget* currWidget = widgetsQueue.front();
      widgetsQueue.pop_front();

      serializeWidgetSettings( *currWidget, save );

      // analyze the widget's children
      const QObjectList& children = currWidget->children();
      foreach( QObject* obj, children )
      {
         QWidget* childWidget = dynamic_cast< QWidget* >( obj );
         if ( childWidget )
         {
            widgetsQueue.push_back( childWidget );
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::serializeWidgetSettings( QWidget& widget, bool save )
{
   QString objName = widget.objectName();
   if ( objName.isEmpty() )
   {
      return;
   }
   m_uiSettings->beginGroup( objName );

   // serialize common widget settings
   if ( save )
   {
      m_uiSettings->setValue( "size", widget.size() );
      m_uiSettings->setValue( "pos", widget.pos() );
   }
   else
   {
      widget.resize( m_uiSettings->value( "size", widget.size() ).toSize() );
      widget.move( m_uiSettings->value( "pos", widget.pos() ).toPoint() );
   }

   // serialize type specific settings (non-exclusive ifs block)
   if ( dynamic_cast< QDockWidget* >( &widget ) )
   {
      serializeDockWidgetSettings( *dynamic_cast< QDockWidget* >( &widget ), save );
   }

   if ( dynamic_cast< QTreeWidget* >( &widget ) )
   {
      serializeTreeWidgetSettings( *dynamic_cast< QTreeWidget* >( &widget ), save );
   }

   // close the widget's settings group
   m_uiSettings->endGroup();
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::serializeDockWidgetSettings( QDockWidget& widget, bool save )
{
   if ( save )
   {
      m_uiSettings->setValue( "features", (int)widget.features() );
      m_uiSettings->setValue( "floating", widget.isFloating() );
   }
   else
   {
      widget.setFeatures( ( QDockWidget::DockWidgetFeatures )m_uiSettings->value( "features", (int)widget.features() ).toInt() );
      widget.setFloating( m_uiSettings->value( "floating", widget.isFloating() ).toBool() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::serializeTreeWidgetSettings( QTreeWidget& widget, bool save )
{
   // gather info about present columns count
   QList< QVariant > columnSizes;
   int count = widget.columnCount();
   for ( int i = 0; i < count; ++i )
   {
      columnSizes.push_back( widget.columnWidth( i ) );
   }

   if ( save )
   {
      m_uiSettings->setValue( "columnSizes", columnSizes );
   }
   else
   {
      columnSizes = m_uiSettings->value( "columnSizes", columnSizes ).toList();

      // set the column sizes
      widget.setColumnCount( columnSizes.size() );
      int colIdx = 0;
      foreach( QVariant width, columnSizes )
      {
         widget.setColumnWidth( colIdx++, width.toInt() );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::registerSubEditor( QMainWindow* editor )
{
   std::vector< QMainWindow* >::iterator it = std::find( m_subEditors.begin(), m_subEditors.end(), editor );
   if ( it == m_subEditors.end() )
   {
      m_subEditors.push_back( editor );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::unregisterSubEditor( QMainWindow& editor )
{
   std::vector< QMainWindow* >::iterator it = std::find( m_subEditors.begin(), m_subEditors.end(), &editor );
   if ( it != m_subEditors.end() )
   {
      m_subEditors.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////
