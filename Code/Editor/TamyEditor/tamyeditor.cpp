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
   std::string rootDir = m_resourceMgr->getFilesystem().getCurrRoot();

   QString fullFileName = QFileDialog::getOpenFileName(this, 
      tr("Load scene"), 
      rootDir.c_str(), 
      tr("Scene Files (*.tsc)"));

   if (fullFileName.isEmpty() == true) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }

   // once the file is open, extract the directory name
   std::string fileName = extractFileName(fullFileName.toStdString(), rootDir);

   // show the progress dialog
   ProgressDialog progressDialog(this);
   progressDialog.show();
   m_scene->clear();

   try
   {
      if (fileName.find(".tsc") != std::string::npos)
      {
         TProgressObserver<Model::ProgressObserver> progressObserver(progressDialog.getProgressBar());

         Filesystem fs(rootDir);
         File* archive = fs.open(fileName, std::ios_base::in | std::ios_base::binary);
         Loader loader(new FileSerializer(archive));
         m_scene->load(loader, progressObserver);
      }
   }
   catch (std::exception& ex)
   {
      QMessageBox::warning(this, "Load scene error",
         QString("Error occurred while loading scene ") + fileName.c_str(),
         QMessageBox::Ok);
   }

   // close the dialog
   progressDialog.hide();
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::saveScene()
{
   std::string rootDir = m_resourceMgr->getFilesystem().getCurrRoot();

   QString fullFileName = QFileDialog::getSaveFileName(this, 
      tr("Save scene"), 
      rootDir.c_str(), 
      tr("Scene Files (*.tsc)"));

   if (fullFileName.isEmpty() == true) 
   {
      // no file was selected or user pressed 'cancel'
      return;
   }
   // once the file is open, extract the directory name
   std::string fileName = extractFileName(fullFileName.toStdString(), rootDir);

   // show the progress dialog
   ProgressDialog progressDialog(this);
   progressDialog.show();
   TProgressObserver<Model::ProgressObserver> progressObserver(progressDialog.getProgressBar());

   Filesystem fs(rootDir);
   File* archive = NULL;
   try
   {
      archive = fs.open(fileName, std::ios_base::out | std::ios_base::binary);
   }
   catch (std::exception& ex)
   {
      QMessageBox::warning(this, "Save scene error",
         QString("Error occurred while saving scene ") + fileName.c_str(),
         QMessageBox::Ok);

      return;
   }

   // import the scene
   Saver saver(new FileSerializer(archive));
   m_scene->save(saver, progressObserver);

   // close the dialog
   progressDialog.hide();
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::importFromIWF()
{
   std::string rootDir = m_resourceMgr->getFilesystem().getCurrRoot();

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
   std::string importFileName = extractFileName(iwfFileName.toStdString(), rootDir);

   // show the progress dialog
   ProgressDialog progressDialog(this);
   progressDialog.show();
   m_scene->clear();

   try
   {
      if (importFileName.find(".iwf") != std::string::npos)
      {
         TProgressObserver<IWFScene::ProgressObserver> progressObserver(progressDialog.getProgressBar());  
         IWFScene& res = m_resourceMgr->create<IWFScene> (importFileName);
         res.load(*m_scene, progressObserver);
      }
   }
   catch (std::exception& ex)
   {
      QMessageBox::warning(this, "Load scene error",
         QString("Error occurred while loading scene ") + importFileName.c_str(),
         QMessageBox::Ok);
   }

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

std::string TamyEditor::extractFileName(const std::string& fileName, 
                                        const std::string& rootDir) const
{
   // tokenize both the filename and the root dir
   std::vector<std::string> fileNameTokens;
   std::vector<std::string> rootDirTokens;

   tokenize(fileName, fileNameTokens, "\\/");
   tokenize(rootDir, rootDirTokens, "\\/");

   // if the root dir path contains more or equal amount of nodes
   // as the filename, file can't possibly be located inside that directory
   if (rootDirTokens.size() >= fileNameTokens.size())
   {
      return "";
   }

   // compare all the nodes from the root directory to those on the file
   // to make sure the file is indeed located inside the root director
   unsigned int rootDirTokensCount = rootDirTokens.size();
   for (unsigned int i = 0; i < rootDirTokensCount; ++i)
   {
      if (rootDirTokens[i] != fileNameTokens[i])
      {
         return "";
      }
   }

   // assemble a file name relative to the root directory
   std::string relativeFileName = ".";
   unsigned int fileNameTokensCount = fileNameTokens.size();
   for (unsigned int i = rootDirTokensCount; i < fileNameTokensCount; ++i)
   {
      relativeFileName += "/" + fileNameTokens[i];
   }
   return relativeFileName;
}

///////////////////////////////////////////////////////////////////////////////

void TamyEditor::tokenize(const std::string& str, 
                          std::vector<std::string>& output,
                          const std::string& tokens) const
{
   std::size_t start, end;
   std::string tmp;
   start = 0;
   end = 0;

   while (end != std::string::npos)
   {
      end = str.find_first_of(tokens, start);
      tmp = str.substr(start, end - start);
      if (tmp.size() > 0)
      {
         output.push_back(tmp);
      }
      start = end + 1;
   }
}

///////////////////////////////////////////////////////////////////////////////
