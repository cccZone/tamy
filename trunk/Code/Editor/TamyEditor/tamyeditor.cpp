#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "core-Renderer.h"
#include "core-MVC.h"
#include "ml-IWF.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QProgressBar.h>
#include <QMessageBox.h>
#include <QTimer.h>
#include "progressdialog.h"

// components
#include "TamySceneWidget.h"
#include "SceneNavigator.h"
#include "SceneQueries.h"
#include "PropertiesEditor.h"
#include "..\src\gui\dialogs\qfiledialog.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class ProgressObserver
   {
   private:
      ProgressDialog*   m_progressDialog;
      QProgressBar&     m_progressBar;

   public:
      ProgressObserver( QWidget* parent )
         : m_progressDialog( new ProgressDialog( parent ) )
         , m_progressBar( m_progressDialog->getProgressBar() )
      {
         m_progressDialog->show();

         m_progressBar.setMaximum(INT_MAX);
         m_progressBar.setMinimum(0);
         m_progressBar.setValue(0);
      }

      ~ProgressObserver()
      {
         m_progressDialog->hide();

         delete m_progressDialog;
         m_progressDialog = NULL;
      }

      void setProgress(float percentage)
      {
         int val = (int)(INT_MAX * percentage);
         m_progressBar.setValue(val);
      }
   };
} // anonymous

///////////////////////////////////////////////////////////////////////////////

TamyEditor::TamyEditor(QApplication& app, QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
, m_app(app)
, m_mainTime(new CTimer())
, m_timeController(new TimeController())
, m_resourceMgr(new ResourcesManager())
{
   // setup user interface
   ui.setupUi(this);
   connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(loadScene()));
   connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(saveScene()));
   connect(ui.actionImportFromIWF, SIGNAL(triggered()), this, SLOT(importFromIWF()));
   connect(ui.actionQuit, SIGNAL(triggered()), &m_app, SLOT(quit()));

   m_mainTimeSlot = new QTimer(this);
   connect(m_mainTimeSlot, SIGNAL(timeout()), this, SLOT(updateMain()));
   m_mainTimeSlot->start(1);

   // register basic services
   registerService<TimeController> (*m_timeController);

   m_scene = new Model();
   registerService<Model> (*m_scene);
   registerService<ResourcesManager> (*m_resourceMgr);

   // add components
   m_resourceMgr->setFilesystem( new Filesystem("D:/Docs/Projects/Tamy/Assets") );
   TamySceneWidget* renderWindow = new TamySceneWidget(ui.renderWindow, 0, *m_resourceMgr);
   addComponent(renderWindow);
   addComponent(new SceneNavigator());
   addComponent(new SceneQueries());
   addComponent(new PropertiesEditor());

   m_resourceMgr->associate<Renderer> (requestService<Renderer> ());
}

///////////////////////////////////////////////////////////////////////////////

TamyEditor::~TamyEditor()
{
   delete m_scene; m_scene = NULL;
   delete m_resourceMgr; m_resourceMgr = NULL;

   delete m_mainTime; m_mainTime = NULL;
   delete m_mainTimeSlot; m_mainTimeSlot = NULL;
   delete m_timeController; m_timeController = NULL;
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

QMenu& TamyEditor::getViewMenu()
{
   return *(ui.menuView);
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::loadScene()
{
   const Filesystem& fs = m_resourceMgr->getFilesystem();
   std::string rootDir = fs.getCurrRoot();
   std::string filter( "Scene files (*." );
   filter += std::string( Model::getExtension() ) + ")";

   QString fullFileName = QFileDialog::getOpenFileName(this, 
      tr("Load scene"), 
      rootDir.c_str(), 
      filter.c_str());

   if (fullFileName.isEmpty() == true) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }

   // once the file is open, extract the directory name
   std::string fileName = fs.toRelativePath( fullFileName.toStdString() );

   try
   {
      ProgressObserver progressObserver( this );
      progressObserver.setProgress( 0 );
      Model& newScene = dynamic_cast< Model& >( m_resourceMgr->create( fileName ) );
      progressObserver.setProgress( 1 );
   }
   catch (std::exception& ex)
   {
      QMessageBox::warning(this, "Load scene error",
         QString("Error occurred while loading scene ") + fileName.c_str(),
         QMessageBox::Ok);
   }

   // TODO: (TOP !!!!!!) rozeslac notyfikacje o nowym modelu
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::saveScene()
{
   try
   {
      ProgressObserver progressObserver( this );
      progressObserver.setProgress( 0 );
      ExternalDependenciesSet externalDependencies;
      m_scene->saveResource( externalDependencies );

      progressObserver.setProgress( 0.5f );

      for ( unsigned int i = 0; i < externalDependencies.size(); ++i )
      {
         externalDependencies[ i ]->saveResource( externalDependencies );
         float progress = (float)i / (float)externalDependencies.size();
         progressObserver.setProgress( progress );
      }

   }
   catch (std::exception& ex)
   {
      QMessageBox::warning(this, "Save scene error",
         QString("Error occurred while saving scene"),
         QMessageBox::Ok);

      return;
   }
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::importFromIWF()
{
   const Filesystem& fs = m_resourceMgr->getFilesystem();
   std::string rootDir = fs.getCurrRoot();

   QString iwfFileName = QFileDialog::getOpenFileName(this, 
      tr("Import scene from IWF"), 
      rootDir.c_str(), 
      tr("Scene Files (*.iwf)"));

   if (iwfFileName.isEmpty() == true) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }

   // once the file is open, extract the directory name
   std::string importFileName = fs.toRelativePath( iwfFileName.toStdString() );

   try
   {
      ProgressObserver progressObserver( this );
      IWFScene res( fs, importFileName );
      res.load( *m_scene, *m_resourceMgr, progressObserver );

   }
   catch (std::exception& ex)
   {
      QMessageBox::warning(this, "Load scene error",
         QString("Error occurred while loading scene ") + importFileName.c_str(),
         QMessageBox::Ok);
   }

   // TODO: (TOP !!!!!!) rozeslac notyfikacje o nowym modelu

}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::updateMain()
{
   m_mainTime->tick();
   float timeElapsed = m_mainTime->getTimeElapsed();

   m_timeController->update(timeElapsed);
}

///////////////////////////////////////////////////////////////////////////////
