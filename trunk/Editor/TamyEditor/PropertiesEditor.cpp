#include "PropertiesEditor.h"
#include "tamyeditor.h"
#include "core-Scene\Entity.h"
#include <QWidget.h>
#include <QDockWidget.h>
#include <QScrollArea.h>
#include <QBoxLayout.h>
#include <QLabel.h>
#include <QMenu.h>
#include <QAction.h>
#include <QtGui\QSpacerItem>

// properties
#include "Vec3PropertyEditor.h"
#include "MtxPropertyEditor.h"
#include "StringPropertyEditor.h"
#include "ColorPropertyEditor.h"
#include "FloatPropertyEditor.h"
#include "IntPropertyEditor.h"

// camera commands
#include "CameraController.h"
#include "SelectEntityCommand.h"


///////////////////////////////////////////////////////////////////////////////

PropertiesEditor::PropertiesEditor()
: m_propertiesLayout(NULL)
{
   associate<TProperty<D3DXVECTOR3>, Vec3PropertyEditor>();
   associate<TProperty<D3DXMATRIX>, MtxPropertyEditor>();
   associate<TProperty<std::string>, StringPropertyEditor>();
   associate<TProperty<Color>, ColorPropertyEditor>();
   associate<TProperty<float>, FloatPropertyEditor<float> >();
   associate<TProperty<double>, FloatPropertyEditor<double> >();
   associate<TProperty<int>, IntPropertyEditor<int> >();
   associate<TProperty<unsigned int>, IntPropertyEditor<unsigned int> >();
   associate<TProperty<long>, IntPropertyEditor<long> >();
   associate<TProperty<unsigned long>, IntPropertyEditor<unsigned long> >();
}

///////////////////////////////////////////////////////////////////////////////

PropertiesEditor::~PropertiesEditor()
{
   m_propertiesLayout = NULL;
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
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::selectEntity(Entity& entity)
{
   reset();

   Properties& properties = entity.properties();
   m_entityClassName->setText(properties.getClassName().c_str());

   set(properties);
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::resetSelection()
{
   reset();
   m_entityClassName->setText("");
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
   verticalLayout->setSpacing(0);
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

   m_propertiesLayout = new QVBoxLayout(scrollArea);
   m_propertiesLayout->setSpacing(6);
   m_propertiesLayout->setMargin(0);
   m_propertiesLayout->setObjectName(QString::fromUtf8("m_propertiesLayout"));

   m_entityClassName = new QLabel(scrollArea);
   m_propertiesLayout->addWidget(m_entityClassName);

   m_spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

   // setup menu entries
   QAction* actionProperties = dockWidget->toggleViewAction();
   actionProperties->setObjectName(QString::fromUtf8("actionProperties"));
   actionProperties->setText(QApplication::translate("TamyEditorClass", "Properties", 0, QApplication::UnicodeUTF8));
   viewMenu.addAction(actionProperties);
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::addPropertyEditor(QWidget* editorWidget)
{
   m_propertiesLayout->removeItem(m_spacer);
   m_propertiesLayout->addWidget(editorWidget);
   m_propertiesLayout->addItem(m_spacer);
}

///////////////////////////////////////////////////////////////////////////////

void PropertiesEditor::removePropertyEditor(QWidget& editorWidget)
{
   m_propertiesLayout->removeWidget(&editorWidget);
}

///////////////////////////////////////////////////////////////////////////////
