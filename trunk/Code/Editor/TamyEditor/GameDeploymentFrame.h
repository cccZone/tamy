/// @file   TamyEditor/GameDeploymentFrame.h
/// @brief  game deployment settings frame
#pragma once

#include <QFrame>
#include <QCloseEvent>
#include "ui_gamedeploymentframe.h"


///////////////////////////////////////////////////////////////////////////////

class SceneEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * Supported deployment platforms
 */
enum GameDeploymentPlatform
{
   GDP_WINDOWS_DX9,
   GDP_WINDOWS_OPENGL,
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A structure that holds vital information telling how the game should be deployed.
 */
struct GameDeploymentInfo
{
   std::string                m_targetDir;
   GameDeploymentPlatform     m_platform;
   bool                       m_runAfterDeployment;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Game deployment settings frame.
 */
class GameDeploymentFrame : public QFrame
{
   Q_OBJECT

private:
    Ui::GameDeploymentFrame               m_ui;

public:
   /**
    * Constructor.
    *
    * @param parentWidget     parent widget
    */
   GameDeploymentFrame( QWidget* parentWidget );
   ~GameDeploymentFrame();

public slots:
   void onSelectPath();
   void onDeploy();

signals:
   /**
    * Emitted when the user hits the 'Deploy' button.
    *
    * @param info       deployment info
    */
   void deployGame( QWidget*, const GameDeploymentInfo& info );
};

///////////////////////////////////////////////////////////////////////////////
