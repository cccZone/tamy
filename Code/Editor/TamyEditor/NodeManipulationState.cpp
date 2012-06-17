#include "NodeManipulationState.h"
#include "NavigationState.h"
#include "GizmoController.h"
#include "TamySceneWidget.h"
#include "SceneEditor.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( NodeManipulationState )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

NodeManipulationState::NodeManipulationState()
   : m_gizmoAxis( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void NodeManipulationState::setGizmoAxis( GizmoAxis& selectedGizmoAxis )
{
   m_gizmoAxis = &selectedGizmoAxis;
}

///////////////////////////////////////////////////////////////////////////////

void NodeManipulationState::activate()
{
   if ( fsm().areNodesSelected() && m_gizmoAxis )
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

void NodeManipulationState::setupController()
{
   SceneObjectsManipulator& controller = fsm();
   controller.setController( new GizmoController( *m_gizmoAxis ) ); 
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
