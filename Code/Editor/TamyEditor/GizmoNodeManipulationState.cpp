#include "GizmoNodeManipulationState.h"
#include "NavigationState.h"
#include "GizmoController.h"
#include "TamySceneWidget.h"
#include "SceneEditor.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GizmoNodeManipulationState )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

GizmoNodeManipulationState::GizmoNodeManipulationState()
   : m_gizmoAxis( NULL )
   , m_uic( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void GizmoNodeManipulationState::initInput( TamySceneWidget& widget )
{
   m_uic = &widget;
}

///////////////////////////////////////////////////////////////////////////////

void GizmoNodeManipulationState::setGizmoAxis( GizmoAxis& selectedGizmoAxis )
{
   m_gizmoAxis = &selectedGizmoAxis;
}

///////////////////////////////////////////////////////////////////////////////

void GizmoNodeManipulationState::activate()
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

void GizmoNodeManipulationState::deactivate()
{
   fsm().setController( NULL );
}

///////////////////////////////////////////////////////////////////////////////

void GizmoNodeManipulationState::execute( float timeElapsed )
{
   if ( !m_uic->isKeyPressed( VK_LBUTTON ) )
   {
      // we released the manipulated object - go back to the navigation mode
      transitionTo< NavigationState >();
   }
}

///////////////////////////////////////////////////////////////////////////////

void GizmoNodeManipulationState::setupController()
{
   SceneObjectsManipulator& controller = fsm();
   controller.setController( new GizmoController( *m_gizmoAxis ) ); 
}

///////////////////////////////////////////////////////////////////////////////
