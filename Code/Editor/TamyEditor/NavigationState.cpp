#include "NavigationState.h"
#include "NodeManipulationState.h"
#include "CameraMovementController.h"
#include "TamySceneWidget.h"
#include "SceneEditor.h"
#include "SelectionManager.h"
#include "core-Renderer/Renderer.h"
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

// TODO: !!! jak sie nacisnie prawy, pokreci kamera, to potem lewy smash nie dziala

bool NavigationState::keySmashed( unsigned char keyCode )
{
   if ( keyCode == VK_LBUTTON )
   {
      TamySceneWidget& sceneWidget = fsm().getSceneWidget();

      // issue an object selection command
      sceneWidget.localToViewport( sceneWidget.getMousePos(), m_queryPos );
      sceneWidget.queryScene( *this );

      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////

void NavigationState::setResult( Entity* foundEntity )
{
   SelectionManager& selectionMgr = fsm().getSceneEditor().getSelectionMgr();
   if ( foundEntity )
   {
      // we found something
      selectionMgr.selectEntity( *foundEntity );
   }
   else
   {
      // we found nothing, so cancel the current selection
      selectionMgr.resetSelection();
   }
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
