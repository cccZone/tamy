#pragma once

/// @file   TamyEditor\SceneRenderer.h
/// @brief  a component for displaying tamy scenes in the main editor window

#include <QWidget>
#include "core/Component.h"
#include "tamyeditor.h"


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class TamySceneWidget;
class Model;

///////////////////////////////////////////////////////////////////////////////

/**
 * Main editor component that allows to view the edited scene.
 */
class SceneRenderer : public Component< TamyEditor >
{
private:
   QWidget*                m_parentWidget;
   TamySceneWidget*        m_sceneWidget;
   Model*                  m_scene;

public:
   /**
    * Constructor.
    *
    * @param parentWidget
    */
   SceneRenderer( QWidget* parentWidget );
   ~SceneRenderer();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );
   void onServiceRegistered( TamyEditor& mgr );

private:
   void createRenderer( TamyEditor& mgr );
};

///////////////////////////////////////////////////////////////////////////////
