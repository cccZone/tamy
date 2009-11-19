#include "PropertiesEditor.h"
#include "tamyeditor.h"
#include <QWidget.h>
#include <QDockWidget.h>
#include <QScrollArea.h>
#include <QBoxLayout.h>
#include <QMenu.h>
#include <QAction.h>


///////////////////////////////////////////////////////////////////////////////

PropertiesEditor::PropertiesEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

PropertiesEditor::~PropertiesEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::initialize(TamyEditor& mgr)
{
   // register new services
   mgr.registerService<PropertiesEditor> (*this);

   // initialize user interface
   initUI(mgr, mgr.getViewMenu());
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::selectEntity(Entity& entity)
{
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::initUI(QMainWindow& mainWindow, QMenu& viewMenu)
{
   // setup dockable properties view widget
   QDockWidget* dockWidget = new QDockWidget(&mainWindow);
   mainWindow.addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);
   dockWidget->setObjectName(QString::fromUtf8("dockWidget"));

   QWidget* dockWidgetContents = new QWidget();
   dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));

   QVBoxLayout* verticalLayout = new QVBoxLayout(dockWidgetContents);
   verticalLayout->setSpacing(6);
   verticalLayout->setMargin(0);
   verticalLayout->setObjectName(QString::fromUtf8("verticalLayout_3"));

   QScrollArea* scrollArea = new QScrollArea(dockWidgetContents);
   scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
   scrollArea->setWidgetResizable(true);

   QWidget* propertiesWidget = new QWidget();
   propertiesWidget->setObjectName(QString::fromUtf8("propertiesWidget"));
   propertiesWidget->setGeometry(QRect(0, 0, 78, 322));
   scrollArea->setWidget(propertiesWidget);

   verticalLayout->addWidget(scrollArea);
   dockWidget->setWidget(dockWidgetContents);
   dockWidget->setWindowTitle(QString::fromUtf8("Properties"));

   // setup menu entries
   QAction* actionProperties = dockWidget->toggleViewAction();
   actionProperties->setObjectName(QString::fromUtf8("actionProperties"));
   actionProperties->setText(QApplication::translate("TamyEditorClass", "Properties", 0, QApplication::UnicodeUTF8));
   viewMenu.addAction(actionProperties);
}

///////////////////////////////////////////////////////////////////////////////
