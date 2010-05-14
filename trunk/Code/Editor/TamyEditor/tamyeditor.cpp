#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "core-Renderer.h"
#include "core-MVC.h"
#include "progressdialog.h"
#include <QTimer.h>

// components
#include "MainAppComponent.h"
#include "SelectionManager.h"
#include "TamySceneWidget.h"
#include "SceneNavigator.h"
#include "SceneQueries.h"
#include "PropertiesEditor.h"
#include "SceneTreeViewer.h"
#include "ResourcesBrowser.h"


// TODO: (TOP 1) entities editor as one of the properties editors

///////////////////////////////////////////////////////////////////////////////

TamyEditor::TamyEditor(QApplication& app, QWidget *parent, Qt::WFlags flags)
: QMainWindow( parent, flags )
, m_mainTime( new CTimer() )
, m_mainAppComponent( new MainAppComponent( app ) )
{
   ui.setupUi(this);

   m_mainTimeSlot = new QTimer(this);
   connect(m_mainTimeSlot, SIGNAL(timeout()), this, SLOT(updateMain()));
   m_mainTimeSlot->start(1);

   // add components
   addComponent( m_mainAppComponent );
   addComponent( new TamySceneWidget( ui.renderWindow, 0 ) );
   addComponent( new SelectionManager() );
   addComponent( new SceneNavigator() );
   addComponent( new SceneQueries() );
   addComponent( new PropertiesEditor() );
   addComponent( new SceneTreeViewer() );
   addComponent( new ResourcesBrowser() );
}

///////////////////////////////////////////////////////////////////////////////

TamyEditor::~TamyEditor()
{
   m_mainAppComponent = NULL;
   delete m_mainTime; m_mainTime = NULL;
   delete m_mainTimeSlot; m_mainTimeSlot = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::addToMainWidget(QWidget* widget)
{
   ui.renderWindow->layout()->addWidget(widget);
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::removeFromMainWidget(QWidget& widget)
{
   ui.renderWindow->layout()->removeWidget(&widget);
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
