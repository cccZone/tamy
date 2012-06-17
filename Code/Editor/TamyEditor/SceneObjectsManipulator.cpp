#include "SceneObjectsManipulator.h"
#include "TamySceneWidget.h"
#include "core-MVC/SpatialEntity.h"

// states
#include "SceneObjectsManipulatorState.h"
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
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::update( float timeElapsed )
{
   FSMController< SceneObjectsManipulator >::update( timeElapsed );

   if ( m_activeController )
   {
      m_activeController->update( timeElapsed );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::keySmashed( unsigned char keyCode )
{
   if ( getCurrentState< SceneObjectsManipulatorState >().keySmashed( keyCode ) )
   {
      return;
   }

   if ( m_activeController )
   {
      m_activeController->keySmashed( keyCode );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::keyHeld( unsigned char keyCode )
{
   if ( getCurrentState< SceneObjectsManipulatorState >().keyHeld( keyCode ) )
   {
      return;
   }

   if ( m_activeController )
   {
      m_activeController->keyHeld( keyCode );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneObjectsManipulator::keyReleased( unsigned char keyCode )
{
   if ( getCurrentState< SceneObjectsManipulatorState >().keyReleased( keyCode ) )
   {
      return;
   }

   if ( m_activeController )
   {
      m_activeController->keyReleased( keyCode );
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
