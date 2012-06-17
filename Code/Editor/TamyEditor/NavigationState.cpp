#include "NavigationState.h"
#include "NodeManipulationState.h"
#include "CameraMovementController.h"
#include "TamySceneWidget.h"
#include "SceneEditor.h"
#include "SelectionManager.h"
#include "GizmoAxis.h"
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
}

///////////////////////////////////////////////////////////////////////////////

void NavigationState::deactivate()
{
   fsm().setController( NULL );
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
   // nothing to do here
   return false;
}

///////////////////////////////////////////////////////////////////////////////

bool NavigationState::keyReleased( unsigned char keyCode )
{
   // nothing to do here
   return false;
}

///////////////////////////////////////////////////////////////////////////////

void NavigationState::setResult( const Array< Entity* >& foundEntities )
{
   // analyze the found entities and determine what ( if any ) spatial entities were found,
   // and whether a gizmo axis was selected
   GizmoAxis* selectedGizmoAxis = NULL;
   Array< SpatialEntity* > selectedNodes;
   {
      uint count = foundEntities.size();
      for ( uint i = 0; i < count; ++i )
      {
         Entity* foundEntity = foundEntities[i];
         if ( !foundEntity )
         {
            continue;
         }

         // first a quick test to see if the selected entity is a gizmo axis
         if ( foundEntity->isA< GizmoAxis >() )
         {
            ASSERT_MSG( selectedGizmoAxis == NULL, "Only one scene with gizmo is allowed with this query listener, and here we've found the second gizmo axis already" );

            // yup - memorize it and go to the next one
            selectedGizmoAxis = DynamicCast< GizmoAxis >( foundEntity );

            // and go to the next item on the list
            continue;
         }

         // next - determine if the entity is a spatial entity
         {
            Entity* spatialEntityCandidate = foundEntity;
            while( spatialEntityCandidate && !spatialEntityCandidate->isA< SpatialEntity >() )
            {
               // find a parent of the entity that's a node
               spatialEntityCandidate = &spatialEntityCandidate->getParent();
            }

            if ( spatialEntityCandidate != NULL )
            {
               // we found it
               SpatialEntity* node = DynamicCast< SpatialEntity >( spatialEntityCandidate );
               selectedNodes.push_back( node );
            }
         }
      }
   }

 
   if ( selectedGizmoAxis )
   {
      // if a gizmo axis was selected, then it means we want to manipulate the selected object.
      // At this point we're no longer interested in any other selected objects

      ASSERT_MSG( fsm().areNodesSelected(), "There are no selected nodes, and yet we managed to select a manipulation gizmo somehow. Check it!" );
      transitionTo< NodeManipulationState >().setGizmoAxis( *selectedGizmoAxis );
   }
   else
   {
      // analyze the selected spatial entities
      analyzeSelectedNodes( selectedNodes );
   }
}

///////////////////////////////////////////////////////////////////////////////

void NavigationState::analyzeSelectedNodes( const Array< SpatialEntity* >& selectedNodes )
{
   SelectionManager& selectionMgr = fsm().getSceneEditor().getSelectionMgr();

   uint count = selectedNodes.size();
   bool somethingSelected = false;
   for( uint i = 0; i < count; ++i )
   {
      // we want the manipulator to select only the scene nodes it can actually manipulate afterwards
      SpatialEntity* foundNode = selectedNodes[i];
      if ( foundNode )
      {
         // got a node to select
         selectionMgr.selectEntity( *foundNode );
         somethingSelected = true;
         break;
      }
   }

   if ( !somethingSelected )
   {
      selectionMgr.resetSelection();
   }
}

///////////////////////////////////////////////////////////////////////////////
