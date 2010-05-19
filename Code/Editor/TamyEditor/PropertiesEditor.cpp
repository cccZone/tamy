#include "PropertiesEditor.h"
#include "tamyeditor.h"
#include "core\Object.h"
#include "core-MVC\Entity.h"
#include <QWidget.h>
#include <QDockWidget.h>
#include <QBoxLayout.h>
#include <QMenu.h>
#include <QAction.h>
#include <QScrollArea.h>
#include "QPropertiesView.h"


///////////////////////////////////////////////////////////////////////////////

PropertiesEditor::PropertiesEditor()
: m_mgr( NULL )
, m_rootView( NULL )
, m_layout( NULL )
, m_selectionManager( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

PropertiesEditor::~PropertiesEditor()
{
   if (m_layout)
   {
      m_layout->removeWidget(m_rootView);
      m_layout = NULL;
   }

   delete m_rootView;
   m_rootView = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::initialize(TamyEditor& mgr)
{
   m_mgr = &mgr;

   // register new services
   mgr.registerService< PropertiesEditor >( *this, *this );

   // initialize user interface
   initUI(mgr, mgr.getViewMenu());
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::onServiceRegistered( TamyEditor& mgr )
{
   if ( mgr.needsUpdate< SelectionManager >( *m_selectionManager ) )
   {
      if ( m_selectionManager )
      {
         m_selectionManager->detach( *this );
      }

      if ( mgr.hasService< SelectionManager >() )
      {
         m_selectionManager = &mgr.requestService< SelectionManager >();
         m_selectionManager->attach( *this );
      }
      else
      {
         m_selectionManager = NULL;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::onObjectSelected( Entity& entity )
{
   if (m_rootView != NULL)
   {
      m_layout->removeWidget(m_rootView);
      delete m_rootView;
   }
   m_rootView = new QPropertiesView( *m_mgr );
   m_layout->addWidget(m_rootView);
   entity.viewProperties( *m_rootView );
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::onObjectDeselected( Entity& entity )
{
   if ( m_rootView != NULL )
   {
      m_layout->removeWidget( m_rootView );
      delete m_rootView;
      m_rootView = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::initUI(QMainWindow& mainWindow, QMenu& viewMenu)
{
   // setup dockable properties view widget
   QDockWidget* dockWidget = new QDockWidget(&mainWindow);
   mainWindow.addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);
   dockWidget->setObjectName( "PropertiesEditor/DockWidget" );
   dockWidget->setWindowTitle( "Properties");

   QScrollArea* scrollableDockWidgetContents = new QScrollArea( dockWidget );
   scrollableDockWidgetContents->setObjectName( "PropertiesEditor/ScrollableDockWidgetContents" );
   dockWidget->setWidget( scrollableDockWidgetContents );

   QWidget* dockWidgetContents = new QWidget( scrollableDockWidgetContents );
   dockWidgetContents->setObjectName( "PropertiesEditor/DockWidgetContents" );
   scrollableDockWidgetContents->setWidget( dockWidgetContents );
   scrollableDockWidgetContents->setWidgetResizable( true );

   m_layout = new QVBoxLayout(dockWidgetContents);
   dockWidgetContents->setLayout(m_layout);
   m_layout->setSpacing(0);
   m_layout->setMargin(0);

   // setup menu entries
   QAction* actionProperties = dockWidget->toggleViewAction();
   actionProperties->setObjectName("PropertiesEditor/actionProperties");
   actionProperties->setText(QApplication::translate("TamyEditorClass", "Properties", 0, QApplication::UnicodeUTF8));
   viewMenu.addAction(actionProperties);
}

///////////////////////////////////////////////////////////////////////////////
