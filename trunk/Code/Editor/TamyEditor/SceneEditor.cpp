#include "SceneEditor.h"
#include "core-MVC.h"
#include "tamyeditor.h"
#include "MainAppComponent.h"


///////////////////////////////////////////////////////////////////////////////

SceneEditor::SceneEditor( Model& scene )
: m_scene( scene )
{
}

///////////////////////////////////////////////////////////////////////////////

void SceneEditor::initialize( TamyEditor& mgr )
{
   MainAppComponent& mainApp = mgr.requestService< MainAppComponent >();
   mainApp.setScene( m_scene );

   // kill the editor
   delete this;
}

///////////////////////////////////////////////////////////////////////////////
