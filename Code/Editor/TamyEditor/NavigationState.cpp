#include "NavigationState.h"
#include "NodeManipulationState.h"
#include "CameraMovementController.h"
#include "TamySceneWidget.h"
#include "core/Point.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RTTI( NavigationState )
END_RTTI
REGISTER_RTTI( NavigationState )

///////////////////////////////////////////////////////////////////////////////

void NavigationState::activate()
{
   fsm().setController( new CameraMovementController() ); 
}

///////////////////////////////////////////////////////////////////////////////

void NavigationState::deactivate()
{
   fsm().setController( NULL );
}

///////////////////////////////////////////////////////////////////////////////

void NavigationState::onSettingsChanged()
{
   // nothing to do here for now
}

///////////////////////////////////////////////////////////////////////////////

bool NavigationState::keySmashed( unsigned char keyCode )
{
   if ( keyCode == VK_LBUTTON )
   {
      TamySceneWidget& sceneWidget = fsm().getSceneWidget();

      // issue an object selection command
      const Point& screenPos = sceneWidget.getMousePos();
      sceneWidget.selectEntityAt( screenPos );

      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool NavigationState::keyHeld( unsigned char keyCode )
{
   if ( keyCode == VK_LBUTTON && fsm().areNodesSelected() )
   {
      transitionTo< NodeManipulationState >();
      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool NavigationState::keyReleased( unsigned char keyCode )
{
   // nothing to do here
   return false;
}

///////////////////////////////////////////////////////////////////////////////
