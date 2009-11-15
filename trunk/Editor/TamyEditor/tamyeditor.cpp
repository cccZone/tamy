#include "tamyeditor.h"
#include "tamy\Tamy.h"
#include "tamy\SimpleTamyConfigurator.h"
#include "core\Timer.h"
#include "core-Renderer\Renderer.h"
#include "ext-SceneImporters\IWFSceneImporter.h"
#include "core-Scene\Model.h"
#include "core\FileSerializer.h"
#include "core-ResourceManagement\IWFLoader.h"
#include "core\Filesystem.h"
#include "QtWindowBuilder.h"
#include "TamySceneWidget.h"
#include "ext-RendererView\RendererView.h"
#include "ext-MotionControllers\UnconstrainedMotionController.h"
#include "core-Renderer\Camera.h"
#include <QTimer.h>
#include <QFileDialog>
#include <QFileInfo>
#include <QProgressBar.h>
#include <QMessageBox.h>
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
, m_timeMeasurement(new CTimer())
, m_rotating(false)
, m_cameraController(NULL)
{
   // setup user interface
   ui.setupUi(this);
   connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(loadScene()));
   connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(saveScene()));
   connect(ui.actionImport, SIGNAL(triggered()), this, SLOT(importScene()));
   connect(ui.actionQuit, SIGNAL(triggered()), &m_app, SLOT(quit()));

   // initialize tamy
   TamySceneWidget* renderWindow = new TamySceneWidget(ui.renderWindow, 0);
   m_renderWinUiController = renderWindow;
   ui.renderWindow->layout()->addWidget(renderWindow);

   QtWindowBuilder windowBuilder(*renderWindow);
   SimpleTamyConfigurator configurator(800, 600, false);
   TAMY.initialize("tamyEditor", configurator, windowBuilder);

   m_timer = new QTimer(this);
   connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
   m_timer->start(1);

   m_renderer = &(TAMY.renderer());

   // create an empty scene
   m_scene = new Model();

   // attach view
   m_renderView = new RendererView::RendererView(TAMY.graphicalFactory(), *m_renderer);
   m_scene->attach(*m_renderView);

   // setup the camera controller
   m_cameraController = new UnconstrainedMotionController(m_renderView->camera());
}

///////////////////////////////////////////////////////////////////////////////

TamyEditor::~TamyEditor()
{
   delete m_timeMeasurement; m_timeMeasurement = NULL;
   delete m_cameraController; m_cameraController = NULL;
   delete m_renderView; m_renderView = NULL;
   delete m_scene; m_scene = NULL;
   delete m_timer; m_timer = NULL;
   m_renderer = NULL;
   m_renderWinUiController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::update()
{
   m_timeMeasurement->tick();
   float timeElapsed = m_timeMeasurement->getTimeElapsed();
   
   m_renderWinUiController->update(timeElapsed);
   handleInput(timeElapsed);
   m_renderView->update(timeElapsed);

   m_renderer->render();
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

void TamyEditor::handleInput(float timeElapsed)
{
   float movementSpeed = 40 * timeElapsed;
   float rotationSpeed = 0.1f * timeElapsed;

   // process the keys
   if (m_renderWinUiController->isKeyPressed('W'))     {m_cameraController->move(m_cameraController->getLookVec()   * movementSpeed);}
   if (m_renderWinUiController->isKeyPressed('S'))   {m_cameraController->move(-m_cameraController->getLookVec()  * movementSpeed);}
   if (m_renderWinUiController->isKeyPressed('A'))   {m_cameraController->move(-m_cameraController->getRightVec() * movementSpeed);}
   if (m_renderWinUiController->isKeyPressed('D'))  {m_cameraController->move(m_cameraController->getRightVec()  * movementSpeed);}

   if (m_renderWinUiController->isKeyPressed(VK_LBUTTON) && (m_rotating == false))
   {
      m_renderWinUiController->setRelativeMouseMovement(true);
      m_rotating = true;
   }
   else if ((m_renderWinUiController->isKeyPressed(VK_LBUTTON) == false) && m_rotating)
   {
      m_renderWinUiController->setRelativeMouseMovement(false);
      m_rotating = false;
   }

   // process the mouse
   if (m_rotating)
   {
      D3DXVECTOR2 mouseSpeed = m_renderWinUiController->getMouseSpeed() * rotationSpeed;
      m_cameraController->rotate(mouseSpeed.y, mouseSpeed.x, 0);
   }
}

///////////////////////////////////////////////////////////////////////////////
