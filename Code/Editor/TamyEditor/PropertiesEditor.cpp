#include "PropertiesEditor.h"
#include "tamyeditor.h"
#include "core\Object.h"
#include <QWidget.h>
#include <QDockWidget.h>
#include <QBoxLayout.h>
#include <QMenu.h>
#include <QAction.h>
#include "QPropertiesView.h"

// camera commands
#include "CameraController.h"
#include "SelectEntityCommand.h"
#include "SelectionMarker.h"


///////////////////////////////////////////////////////////////////////////////

PropertiesEditor::PropertiesEditor()
: m_rootView( NULL )
, m_layout( NULL )
, m_selectionMarker( NULL )
, m_selectedEntity( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

PropertiesEditor::~PropertiesEditor()
{
   m_selectedEntity = NULL;
   m_selectionMarker = NULL;

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
   // register new services
   mgr.registerService<PropertiesEditor> (*this);

   // initialize user interface
   initUI(mgr, mgr.getViewMenu());

   // configure input commands
   QueryableScene& scene = mgr.requestService<QueryableScene> ();
   CameraController& camController = mgr.requestService<CameraController> ();
   Camera& camera = mgr.requestService<Camera> ();

   camController.addButtonPressCommand(VK_LBUTTON, new SelectEntityCommand(camera, scene, *this));

   ResourcesManager& resMgr = mgr.requestService<ResourcesManager> ();
   m_selectionMarker = SelectionMarker::create(resMgr);
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::selectObject(Object& object)
{
   if (m_rootView != NULL)
   {
      m_layout->removeWidget(m_rootView);
      delete m_rootView;
   }
   m_rootView = new QPropertiesView();
   m_layout->addWidget(m_rootView);
   object.viewProperties(*m_rootView);


   Entity* entity = dynamic_cast< Entity* >( &object );
   visualizeSelection( entity );
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::resetSelection()
{
   if ( m_rootView != NULL )
   {
      m_layout->removeWidget( m_rootView );
      delete m_rootView;
      m_rootView = NULL;
   }

   visualizeSelection( NULL );
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::visualizeSelection( Entity* newSelection )
{
   if ( m_selectedEntity )
   {
      m_selectedEntity->remove( *m_selectionMarker, false );
   }

   m_selectedEntity = newSelection;

   if ( m_selectedEntity )
   {
      m_selectedEntity->add( m_selectionMarker );
   }
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::initUI(QMainWindow& mainWindow, QMenu& viewMenu)
{
   // setup dockable properties view widget
   QDockWidget* dockWidget = new QDockWidget(&mainWindow);
   mainWindow.addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);
   dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
   dockWidget->setWindowTitle(QString::fromUtf8("Properties"));

   QWidget* dockWidgetContents = new QWidget();
   dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
   dockWidget->setWidget(dockWidgetContents);

   m_layout = new QVBoxLayout(dockWidgetContents);
   dockWidgetContents->setLayout(m_layout);
   m_layout->setSpacing(0);
   m_layout->setMargin(0);

   // setup menu entries
   QAction* actionProperties = dockWidget->toggleViewAction();
   actionProperties->setObjectName(QString::fromUtf8("actionProperties"));
   actionProperties->setText(QApplication::translate("TamyEditorClass", "Properties", 0, QApplication::UnicodeUTF8));
   viewMenu.addAction(actionProperties);
}

///////////////////////////////////////////////////////////////////////////////
