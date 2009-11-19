#include "tamyeditor.h"
#include "tamy.h"
#include "ext-SceneImporters.h"
#include "core-AppFlow.h"
#include "core-Scene.h"
#include "core-ResourceManagement.h"
#include "QtWindowBuilder.h"
#include "TamySceneWidget.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QProgressBar.h>
#include <QMessageBox.h>
#include <QTimer.h>
#include "progressdialog.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   template <typename INTERFACE>
   class TProgressObserver : public INTERFACE
   {
   private:
      QProgressBar& m_progressBar;

   public:
      TProgressObserver(QProgressBar& progressBar)
         : m_progressBar(progressBar)
      {
         m_progressBar.setMaximum(INT_MAX);
         m_progressBar.setMinimum(0);
         m_progressBar.setValue(0);
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
{
   // setup user interface
   ui.setupUi(this);
   connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(loadScene()));
   connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(saveScene()));
   connect(ui.actionImport, SIGNAL(triggered()), this, SLOT(importScene()));
   connect(ui.actionQuit, SIGNAL(triggered()), &m_app, SLOT(quit()));

   m_mainTimeSlot = new QTimer(this);
   connect(m_mainTimeSlot, SIGNAL(timeout()), this, SLOT(updateMain()));
   m_mainTimeSlot->start(1);

   // initialize tamy
   TamySceneWidget* renderWindow = new TamySceneWidget(ui.renderWindow, 0);
   QtWindowBuilder windowBuilder(*renderWindow);
   SimpleTamyConfigurator configurator(800, 600, false);
   TAMY.initialize("tamyEditor", configurator, windowBuilder);

   // register basic services
   registerService<Renderer> (TAMY.renderer());
   registerService<GraphicalEntitiesFactory> (TAMY.graphicalFactory());
   registerService<TimeController> (*m_timeController);

   m_scene = new Model();
   registerService<Model> (*m_scene);

   // add mandatory components
   add(renderWindow);
}

///////////////////////////////////////////////////////////////////////////////

TamyEditor::~TamyEditor()
{
   delete m_scene; m_scene = NULL;

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
   QString fullFileName = QFileDialog::getOpenFileName(this, 
      tr("Load scene"), 
      ".", 
      tr("Scene Files (*.tsc)"));

   if (fullFileName.isEmpty() == true) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }

   // once the file is open, extract the directory name
   QFileInfo fileInfo(fullFileName);
   QString sceneRootDir = fileInfo.absolutePath();
   QString fileName = fileInfo.fileName();

   // set the new filesystem
   std::string oldFilesystemRoot = TAMY.filesystem().getCurrRoot();
   TAMY.filesystem().changeRootDir(sceneRootDir.toStdString());
   File* archive = NULL;
   try
   {
      archive = TAMY.filesystem().open(fileName.toStdString(), std::ios_base::in | std::ios_base::binary);
   }
   catch (std::exception& ex)
   {
      TAMY.filesystem().changeRootDir(oldFilesystemRoot);

      QMessageBox::warning(this, "Load scene error",
         QString("Error occurred while loading scene ") + fileName,
         QMessageBox::Ok);

      return;
   }

   // show the progress dialog
   ProgressDialog progressDialog(this);
   progressDialog.show();

   // import the scene
   m_scene->clear();
   FileSerializer sceneSerializer(archive);
   m_scene->load(sceneSerializer,
      new TProgressObserver<Model::ProgressObserver>(progressDialog.getProgressBar()));

   // close the dialog
   progressDialog.hide();
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::saveScene()
{
   QString fullFileName = QFileDialog::getSaveFileName(this, 
      tr("Save scene"), 
      ".", 
      tr("Scene Files (*.tsc)"));

   if (fullFileName.isEmpty() == true) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }
   // once the file is open, extract the directory name
   QFileInfo fileInfo(fullFileName);
   QString saveRootDir = fileInfo.absolutePath();
   QString fileName = fileInfo.fileName();

   // show the progress dialog
   ProgressDialog progressDialog(this);
   progressDialog.show();
   TProgressObserver<Model::ProgressObserver> progressObserver(progressDialog.getProgressBar());

   std::string oldFilesystemRoot = TAMY.filesystem().getCurrRoot();
   TAMY.filesystem().changeRootDir(saveRootDir.toStdString());
   File* archive = NULL;
   try
   {
      archive = TAMY.filesystem().open(fileName.toStdString(), std::ios_base::out | std::ios_base::binary);
   }
   catch (std::exception& ex)
   {
      TAMY.filesystem().changeRootDir(oldFilesystemRoot);

      QMessageBox::warning(this, "Save scene error",
         QString("Error occurred while saving scene ") + fileName,
         QMessageBox::Ok);

      return;
   }

   // import the scene
   FileSerializer sceneSerializer(archive);
   m_scene->save(sceneSerializer,
      new TProgressObserver<Model::ProgressObserver>(progressDialog.getProgressBar()));

   // close the dialog & go back to the old filesystem
   progressDialog.hide();
   TAMY.filesystem().changeRootDir(oldFilesystemRoot);
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::importScene()
{
   QString fullFileName = QFileDialog::getOpenFileName(this, 
      tr("Import scene"), 
      ".", 
      tr("Scene Files (*.iwf)"));

   if (fullFileName.isEmpty() == true) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }

   // once the file is open, extract the directory name
   QFileInfo fileInfo(fullFileName);
   QString sceneRootDir = fileInfo.absolutePath();
   QString fileName = fileInfo.fileName();

   // set the new filesystem
   TAMY.filesystem().changeRootDir(sceneRootDir.toStdString());

   // show the progress dialog
   ProgressDialog progressDialog(this);
   progressDialog.show();
   TProgressObserver<IWFLoader::ProgressObserver> progressObserver(progressDialog.getProgressBar());

   // import the scene
   m_scene->clear();

   IWFSceneImporter sceneImporter(*m_scene);
   IWFLoader iwfFileLoader(sceneImporter);
   iwfFileLoader.attach(progressObserver);
   iwfFileLoader.load(TAMY.filesystem(), fileName.toStdString());

   // close the dialog
   progressDialog.hide();
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::updateMain()
{
   m_mainTime->tick();
   float timeElapsed = m_mainTime->getTimeElapsed();

   m_timeController->update(timeElapsed);
}

///////////////////////////////////////////////////////////////////////////////
