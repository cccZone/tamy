#include "tamyeditor.h"
#include "tamy.h"
#include "ext-SceneImporters.h"
#include "core-AppFlow.h"
#include "core-Scene.h"
#include "core-ResourceManagement.h"
#include "ext-RendererView.h"
#include "ext-MotionControllers.h"
#include "core-Renderer.h"
#include "QtWindowBuilder.h"
#include "TamySceneWidget.h"
#include <QTimer.h>
#include <QFileDialog>
#include <QFileInfo>
#include <QProgressBar.h>
#include <QMessageBox.h>
#include "progressdialog.h"
#include "EntitiesStorageView.h"
#include "MousePropertiesView.h"

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

   // -------------------------------------------------------------------------

   class CameraMoventCommands : public KeyStatusHandler
   {
   private:
      UnconstrainedMotionController& m_cameraController;
      UserInputController& m_uiController;
      CTimer& m_timer;
      bool m_rotating;

   public:
      CameraMoventCommands(UnconstrainedMotionController& cameraController,
                           UserInputController& uiController,
                           CTimer& timer)
      : m_cameraController(cameraController)
      , m_uiController(uiController)
      , m_timer(timer)
      , m_rotating(false)
      {
      }

      void keySmashed(unsigned char keyCode) 
      {
         interpretInput(keyCode);
      }

      void keyHeld(unsigned char keyCode)
      {
         if (keyCode == VK_RBUTTON)
         {
            m_rotating = true;
            m_uiController.setRelativeMouseMovement(true);
         }

         interpretInput(keyCode);
      }

      void keyReleased(unsigned char keyCode) 
      {
         if (keyCode == VK_RBUTTON)
         {
            m_rotating = false;
            m_uiController.setRelativeMouseMovement(false);
         }
      }

   private:
      void interpretInput(unsigned char keyCode)
      {
         float timeElapsed = m_timer.getTimeElapsed();
         float movementSpeed = 40 * timeElapsed;
         float rotationSpeed = 0.1f * timeElapsed;

         // process the keys
         if (keyCode == 'W') {m_cameraController.move(m_cameraController.getLookVec()   * movementSpeed);}
         if (keyCode == 'S') {m_cameraController.move(-m_cameraController.getLookVec()  * movementSpeed);}
         if (keyCode == 'A') {m_cameraController.move(-m_cameraController.getRightVec() * movementSpeed);}
         if (keyCode == 'D') {m_cameraController.move(m_cameraController.getRightVec()  * movementSpeed);}

         if (m_rotating)
         {
            D3DXVECTOR2 mouseSpeed = m_uiController.getMouseSpeed() * rotationSpeed;
            m_cameraController.rotate(mouseSpeed.y, mouseSpeed.x, 0);
         }
      }
   };

   // -------------------------------------------------------------------------

   class EntitiesSelector : public KeyStatusHandler
   {
   private:
      Renderer& m_renderer;
      Camera& m_camera;
      SpatialStorage<SpatiallyQueryable>& m_storage;
      UserInputController& m_uiController;
      PropertiesView& m_selectionManager;

   public:
      EntitiesSelector(Renderer& renderer, 
                       Camera& camera,
                       SpatialStorage<SpatiallyQueryable>& storage,
                       UserInputController& uiController,
                       PropertiesView& selectionManager)
      : m_renderer(renderer)
      , m_camera(camera)
      , m_storage(storage)
      , m_uiController(uiController)
      , m_selectionManager(selectionManager)
      {
      }

      void keySmashed(unsigned char keyCode) 
      {
      }

      void keyHeld(unsigned char keyCode)
      {
         if (keyCode != VK_LBUTTON) {return;}

         m_selectionManager.reset();

         D3DXVECTOR2 viewportPos;
         m_renderer.screenToViewport(m_uiController.getMousePos(), viewportPos);

         Ray queryRay = m_camera.createRay(viewportPos.x, viewportPos.y);

         // perform the query
         Array<SpatiallyQueryable*> queryables;
         m_storage.query(queryRay, queryables);

         if (queryables.size() == 0) {return;}

         SpatiallyQueryable* queryable = findClosest(queryRay, queryables);
         m_selectionManager.set(dynamic_cast<Entity*> (queryable)->properties());
      }

      void keyReleased(unsigned char keyCode) 
      {
      }

   private:
      SpatiallyQueryable* findClosest(const Ray& ray,
                                      const Array<SpatiallyQueryable*>& queryables)
      {
         float minDist = FLT_MAX;
         SpatiallyQueryable* closest = NULL;

         unsigned int count = queryables.size();
         for (unsigned int i = 0; i < count; ++i)
         {
            float dist = ray.getDistanceTo(queryables[i]->getBoundingVolume());

            if (dist < minDist)
            {
               minDist = dist;
               closest = queryables[i];
            }
         }

         return closest;
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TamyEditor::TamyEditor(QApplication& app, QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
, m_app(app)
, m_mainTime(new CTimer())
, m_inputTime(new CTimer())
, m_keysStatusManager(NULL)
, m_cameraController(NULL)
, m_entitiesQueryStorage(NULL)
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

   m_mainTimeSlot = new QTimer(this);
   connect(m_mainTimeSlot, SIGNAL(timeout()), this, SLOT(updateMain()));
   m_mainTimeSlot->start(1);

   m_inputTimeSlot = new QTimer(this);
   connect(m_inputTimeSlot, SIGNAL(timeout()), this, SLOT(updateInput()));
   m_inputTimeSlot->start(1);

   m_renderer = &(TAMY.renderer());

   // setup mouse selection view
   m_selectionManager = new MousePropertiesView();

   // create an empty scene
   m_scene = new Model();

   // attach view
   m_renderView = new RendererView::RendererView(TAMY.graphicalFactory(), *m_renderer);
   m_scene->attach(*m_renderView);

   // setup the camera controller
   m_cameraController = new UnconstrainedMotionController(m_renderView->camera());

   // set up the queryable scene storage
   AABoundingBox sceneBB(D3DXVECTOR3(-2000, -2000, -2000), D3DXVECTOR3(2000, 2000, 2000));
   EntitiesStorageView* entitiesQueryView = new EntitiesStorageView(sceneBB);
   m_scene->attach(*entitiesQueryView);

   // filter will take care of deleting 'entitiesQueryView'
   NarrowPhaseStorageFilter<SpatiallyQueryable>* filteredStorage = 
      new NarrowPhaseStorageFilter<SpatiallyQueryable>(entitiesQueryView);
   m_entitiesQueryStorage = filteredStorage;

   // setup renderer input handler
   m_keysStatusManager = new KeysStatusManager(*m_renderWinUiController);
   m_keysStatusManager->addHandler(new CameraMoventCommands(*m_cameraController, 
      *m_renderWinUiController, 
      *m_mainTime));
   m_keysStatusManager->addHandler(new EntitiesSelector(*m_renderer,
      m_renderView->camera(),
      *m_entitiesQueryStorage,
      *m_renderWinUiController,
      *m_selectionManager));
}

///////////////////////////////////////////////////////////////////////////////

TamyEditor::~TamyEditor()
{
   delete m_selectionManager; m_selectionManager = NULL;
   delete m_keysStatusManager; m_keysStatusManager = NULL;
   delete m_entitiesQueryStorage; m_entitiesQueryStorage = NULL;
   delete m_cameraController; m_cameraController = NULL;
   delete m_renderView; m_renderView = NULL;
   delete m_scene; m_scene = NULL;

   delete m_mainTime; m_mainTime = NULL;
   delete m_mainTimeSlot; m_mainTimeSlot = NULL;

   delete m_inputTime; m_inputTime = NULL;
   delete m_inputTimeSlot; m_inputTimeSlot = NULL;

   m_renderer = NULL;
   m_renderWinUiController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::updateMain()
{
   m_mainTime->tick();
   float timeElapsed = m_mainTime->getTimeElapsed();
  
   m_renderView->update(timeElapsed);

   m_renderer->render();
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::updateInput()
{
   m_inputTime->tick();
   float timeElapsed = m_inputTime->getTimeElapsed();

   m_renderWinUiController->update(timeElapsed);
   m_keysStatusManager->update(timeElapsed);
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

void TamyEditor::performSceneQuery(const Point& mousePos, Array<SpatiallyQueryable*>& nodes)
{
   D3DXVECTOR2 viewportPos;
   m_renderer->screenToViewport(mousePos, viewportPos);

   Ray queryRay = m_renderView->camera().createRay(viewportPos.x, viewportPos.y);

   // perform the query
   Array<RenderableNode*> queriedNodes;
   m_entitiesQueryStorage->query(queryRay, nodes);
}

///////////////////////////////////////////////////////////////////////////////
