#include "NodeManipulationState.h"
#include "NavigationState.h"
#include "NodeTransformController.h"
#include "TamySceneWidget.h"
#include "SceneEditor.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( NodeManipulationState )
END_OBJECT()


///////////////////////////////////////////////////////////////////////////////

void NodeManipulationState::activate()
{
   if ( fsm().areNodesSelected() )
   {
      setupController();
   }
   else
   {
      transitionTo< NavigationState >();
   }
}

///////////////////////////////////////////////////////////////////////////////

void NodeManipulationState::deactivate()
{
   fsm().setController( NULL );
}

///////////////////////////////////////////////////////////////////////////////

void NodeManipulationState::onSettingsChanged()
{
   // reset the node transform controller
   setupController();
}

///////////////////////////////////////////////////////////////////////////////

void NodeManipulationState::setupController()
{
   SceneObjectsManipulator& controller = fsm();

   SceneEditor& sceneEditor = controller.getSceneEditor();
   NodeTransformControlMode manipulationMode = sceneEditor.getObjectsManipulationMode();

   controller.setController( new NodeTransformController( controller.getSelectedNodes(), manipulationMode ) ); 
}

///////////////////////////////////////////////////////////////////////////////

bool NodeManipulationState::keySmashed( unsigned char keyCode )
{
   // nothing to do here
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool NodeManipulationState::keyHeld( unsigned char keyCode )
{
   // nothing to do here
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool NodeManipulationState::keyReleased( unsigned char keyCode )
{
   if ( keyCode == VK_LBUTTON )
   {
      // we released the manipulated object - go back to the navigation mode
      transitionTo< NavigationState >();
      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////
