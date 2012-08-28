#include "SceneObjectsManipulator.h"
#include "TamySceneWidget.h"
#include "core-MVC/SpatialEntity.h"
#include "SceneEditor.h"
#include "SelectionManager.h"

// states
#include "NavigationState.h"
#include "NodeManipulationState.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SceneObjectsManipulator );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

SceneObjectsManipulator::SceneObjectsManipulator( SceneEditor& editor )
   : m_sceneEditor( editor )
   , m_widget( NULL )
   , m_activeController( NULL )
{
   registerState< NavigationState >();
   registerState< NodeManipulationState >();
   begin< NavigationState >();
}

///////////////////////////////////////////////////////////////////////////////

SceneObjectsManipulator::~SceneObjectsManipulator()
{
   delete m_activeController;
   m_activeController = NULL;

   m_selectedNodes.clear();
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::setController( SceneRendererInputController* controller )
{
   delete m_activeController;

   m_activeController = controller;
   if ( m_activeController )
   {
      m_activeController->initialize( *m_widget );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::initialize( TamySceneWidget& widget )
{
   m_widget = &widget;

   // initialize the states as input receivers
   getState< NavigationState >().initInput( widget );
   getState< NodeManipulationState >().initInput( widget );
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::update( float timeElapsed )
{
   FSMController< SceneObjectsManipulator >::update( timeElapsed );

   if ( m_activeController )
   {
      m_activeController->update( timeElapsed );
   }

   // check if any other keys were pressed and execute actions assigned to them
   if ( m_widget->isKeyPressed( VK_DELETE ) )
   {
      deleteSelectedEntities();
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::onEntitySelected( Entity& entity )
{
   // check if this is a node
   SpatialEntity* node = DynamicCast< SpatialEntity >( &entity );
   if ( !node )
   {
      return;
   }

   // check if we don't have it already on our list
   unsigned int count = m_selectedNodes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_selectedNodes[i] == node )
      {
         // yup - we already have it
         return;
      }
   }

   m_selectedNodes.push_back( node );
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::onEntityDeselected( Entity& entity )
{
   unsigned int count = m_selectedNodes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_selectedNodes[i] == &entity )
      {
         m_selectedNodes.erase( m_selectedNodes.begin() + i );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::deleteSelectedEntities()
{
   SelectionManager& selectionMgr = m_sceneEditor.getSelectionMgr();
   std::vector< Entity* > selectedEntities = selectionMgr.getSelectedEntities();
   selectionMgr.resetSelection();

   // Analyze all selected entities and select only the ones that are the topmost parents
   // among the selected entities.
   // In other words - delete only the entities that don't have their parents ( immediate or not ) listed
   // as a selected entity.
   
   uint selectedEntitiesCount = selectedEntities.size();

   std::set< Entity* > allUniqueChildren;
   {
      Array< Entity* > allChildren;
      for ( uint entityIdx = 0; entityIdx < selectedEntitiesCount; ++entityIdx )
      {
         Entity* entity = selectedEntities[entityIdx];
         entity->collectChildren( allChildren );
      }

      // remove duplicates
      uint allChildrenCount = allChildren.size();
      for ( uint i = 0; i < allChildrenCount; ++i )
      {
         allUniqueChildren.insert( allChildren[i] );
      }
   }

   // remove all children from the list of the selected entities
   for ( int entityIdx = selectedEntitiesCount - 1; entityIdx >= 0; --entityIdx )
   {
      Entity* entity = selectedEntities[entityIdx];
      if ( allUniqueChildren.find( entity ) != allUniqueChildren.end() )
      {
         // this is one of the children - it's gonna get deleted either way, so skip it
         selectedEntities.erase( selectedEntities.begin() + entityIdx );
      }
   }

   // now that we only have the topmost entities, it's time to remove them
   selectedEntitiesCount = selectedEntities.size();
   for ( uint entityIdx = 0; entityIdx < selectedEntitiesCount; ++entityIdx )
   {
      Entity* entity = selectedEntities[entityIdx];
      Entity* parent = entity->m_parent;
      if ( parent )
      {
         parent->remove( *entity );
      }
      else
      {
         m_sceneEditor.getScene().remove( *entity );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

