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
   , m_uic( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void NodeManipulationState::initInput( TamySceneWidget& widget )
{
   m_uic = &widget;
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

void NodeManipulationState::execute( float timeElapsed )
{
   if ( !m_uic->isKeyPressed( VK_LBUTTON ) )
   {
      // we released the manipulated object - go back to the navigation mode
      transitionTo< NavigationState >();
   }
}

///////////////////////////////////////////////////////////////////////////////

void NodeManipulationState::setupController()
{
   SceneObjectsManipulator& controller = fsm();
   controller.setController( new GizmoController( *m_gizmoAxis ) ); 
}

///////////////////////////////////////////////////////////////////////////////
