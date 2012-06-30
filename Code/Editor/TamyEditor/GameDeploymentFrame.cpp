#include "GameDeploymentFrame.h"
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QFileDialog>
#include "core.h"
#include "core-Renderer.h"
#include "core-MVC.h"
#include "SceneEditor.h"


///////////////////////////////////////////////////////////////////////////////

GameDeploymentFrame::GameDeploymentFrame( QWidget* parentWidget )
   : QFrame( parentWidget )
{
   m_ui.setupUi( this );

   connect( m_ui.deploymentPathButton, SIGNAL( clicked() ), this, SLOT( onSelectPath() ) );
   connect( m_ui.deployButton, SIGNAL( clicked() ), this, SLOT( onDeploy() ) );
}

///////////////////////////////////////////////////////////////////////////////

GameDeploymentFrame::~GameDeploymentFrame()
{
}

///////////////////////////////////////////////////////////////////////////////

void GameDeploymentFrame::onSelectPath()
{
   const std::string& fsRootDir = ResourcesManager::getInstance().getFilesystem().getCurrRoot();

   QString dirName = QFileDialog::getExistingDirectory( this, "Select target deployment directory", fsRootDir.c_str(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
   m_ui.deploymentPathText->setText( dirName );
}

///////////////////////////////////////////////////////////////////////////////

void GameDeploymentFrame::onDeploy()
{
   // prepare the deployment info structure
   GameDeploymentInfo info;
   {
      // set the target directory
      info.m_targetDir = m_ui.deploymentPathText->text().toStdString();

      // set the deployment platform ( by default it's always Windows / DX9 )
      info.m_platform = GDP_WINDOWS_DX9;
      if ( m_ui.windowsDXRadio->isChecked() )
      {
         info.m_platform = GDP_WINDOWS_DX9;
      }
      else
      {
         info.m_platform = GDP_WINDOWS_OPENGL;
      }
   }

   // do we want to run the game after it's been successfully deployed?
   info.m_runAfterDeployment = m_ui.runAfterDeploymentCheckBox->isChecked();

   // emit the signal
   emit deployGame( m_ui.deployButton, info );
}

///////////////////////////////////////////////////////////////////////////////
