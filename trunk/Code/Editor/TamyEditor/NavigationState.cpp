#include "NavigationState.h"
#include "NodeManipulationState.h"
#include "CameraMovementController.h"
#include "TamySceneWidget.h"
#include "SceneEditor.h"
#include "SelectionManager.h"
#include "core-Renderer/Renderer.h"
#include "core/Point.h"
#include "core-MVC/SpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( NavigationState )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

void NavigationState::activate()
{
   fsm().setController( new CameraMovementController() ); 
  
   // If something was already selected, make it a selection candidate.
   // However - this state supports the selection of just one node ( for now ) - and
   // thus if more than one entity is selected - don't consider any of them a selection candidate
   SelectionManager& selectionMgr = fsm().getSceneEditor().getSelectionMgr();
   const std::vector< Entity* >& selectedEntities = selectionMgr.getSelectedEntities();
   if ( selectedEntities.size() != 1 )
   {
      m_selectionCandidate = NULL;
   }
   else
   {
      m_selectionCandidate = DynamicCast< SpatialEntity >( selectedEntities[0] );
   }
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
      sceneWidget.localToViewport( sceneWidget.getMousePos(), m_queryPos );
      sceneWidget.queryScene( *this );

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

void NavigationState::setResult( Entity* foundEntity )
{
   // we want the manipulator to select only the scene nodes it can actually manipulate afterwards
   SpatialEntity* foundNode = NULL;
   if ( foundEntity )
   {
      while( foundEntity && !foundEntity->isA< SpatialEntity >() )
      {
         // find a parent of the entity that's a node
         foundEntity = &foundEntity->getParent();
      }

      if ( foundEntity != NULL )
      {
         // we found it
         foundNode = DynamicCast< SpatialEntity >( foundEntity );
      }
   }

   SelectionManager& selectionMgr = fsm().getSceneEditor().getSelectionMgr();
   if ( foundNode )
   {
      if ( m_selectionCandidate != foundNode )
      {
         // something's already selected - we don't want to discard it so easily, so let's 
         // memorize our choice and if the user clicks again, it means he really wants to 
         // select that thing.
         // otherwise - he may just have selected something from the scene tree and wants to manipulate it
         m_selectionCandidate = foundNode;
      }
      else
      {
         // either nothing's selected, or the user has clicked the same object twice, confirming that he
         // wants to select it
         selectionMgr.selectEntity( *foundNode );
         m_selectionCandidate = NULL;
      }
   }
   else
   {
      // we found nothing, so cancel the current selection
      selectionMgr.resetSelection();
   }
}

///////////////////////////////////////////////////////////////////////////////
