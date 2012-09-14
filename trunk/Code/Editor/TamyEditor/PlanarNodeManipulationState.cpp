#include "PlanarNodeManipulationState.h"
#include "NavigationState.h"
#include "TamySceneWidget.h"
#include "SceneEditor.h"
#include "CameraPlaneNodeManipulator.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/Camera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( PlanarNodeManipulationState )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

PlanarNodeManipulationState::PlanarNodeManipulationState()
   : m_uic( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void PlanarNodeManipulationState::initInput( TamySceneWidget& widget )
{
   m_uic = &widget;
}

///////////////////////////////////////////////////////////////////////////////

void PlanarNodeManipulationState::activate()
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

void PlanarNodeManipulationState::deactivate()
{
   fsm().setController( NULL );
}

///////////////////////////////////////////////////////////////////////////////

void PlanarNodeManipulationState::execute( float timeElapsed )
{
   if ( !m_uic->isKeyPressed( VK_LBUTTON ) )
   {
      // we released the manipulated object - go back to the navigation mode
      transitionTo< NavigationState >();
   }
}

///////////////////////////////////////////////////////////////////////////////

void PlanarNodeManipulationState::setupController()
{
   SceneObjectsManipulator& controller = fsm();
   controller.setController( new CameraPlaneNodeManipulator( controller.getSelectedNodes() ) ); 
}

///////////////////////////////////////////////////////////////////////////////
