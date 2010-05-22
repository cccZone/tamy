#include "SelectionManager.h"
#include "SelectionMarker.h"

///////////////////////////////////////////////////////////////////////////////

SelectionManager::SelectionManager()
: m_selectedEntity( NULL )
, m_selectionMarker( NULL )
, m_observedScene( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::initialize( TamyEditor& mgr )
{
   mgr.registerService< SelectionManager >( *this, *this );

   ResourcesManager& resMgr = mgr.requestService<ResourcesManager> ();
   m_selectionMarker = SelectionMarker::create(resMgr);
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::onServiceRegistered( TamyEditor& mgr )
{
   // scene
   if ( mgr.needsUpdate< Model >( *m_observedScene ) )
   {
      if ( m_observedScene )
      {
         m_observedScene->detach( *this );
      }

      if ( mgr.hasService< Model >() )
      {
         m_observedScene = &mgr.requestService< Model >();
         m_observedScene->attach( *this );
      }
      else
      {
         m_observedScene = NULL;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::selectObject( Entity& entity )
{
   resetSelection();
   m_selectedEntity = &entity;

   if ( m_selectedEntity )
   {
      notifyEntitySelected( *m_selectedEntity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::resetSelection()
{
   if ( m_selectedEntity )
   {
      notifyEntityDeselected( *m_selectedEntity );
   }

   m_selectedEntity = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::attach( SelectionManagerListener& listener )
{
   // we don't want duplicated listeners - check if we're trying to add one
   unsigned int count = m_listeners.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_listeners[i] == &listener )
      {
         return;
      }
   }

   // add the new listener
   m_listeners.push_back( &listener );
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::detach( SelectionManagerListener& listener )
{
   for ( Listeners::iterator it = m_listeners.begin(); it != m_listeners.end(); ++it )
   {
      if ( *it == &listener )
      {
         m_listeners.erase( it );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::notifyEntitySelected( Entity& entity )
{
   unsigned int count = m_listeners.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_listeners[i]->onObjectSelected( entity );
   }

   visualizeSelection( &entity );
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::notifyEntityDeselected( Entity& entity )
{
   unsigned int count = m_listeners.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_listeners[i]->onObjectDeselected( entity );
   }

   visualizeSelection( NULL );
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::visualizeSelection( Entity* newSelection )
{/*
   // TODO: przerobic tak zeby tu byla odrebna scena, na ktorej tworzymy
   // kopie zaznaczonego obiektu i dodajemy my marker
   if ( m_selectedEntity )
   {
      m_selectedEntity->remove( *m_selectionMarker, false );
   }

   m_selectedEntity = newSelection;

   if ( m_selectedEntity )
   {
      m_selectedEntity->add( m_selectionMarker );
   }*/
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::onEntityAdded( Entity& entity )
{
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::onEntityRemoved( Entity& entity )
{
   if ( m_selectedEntity == &entity )
   {
      m_selectedEntity = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::onEntityChanged( Entity& entity )
{
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::resetContents()
{
   m_selectedEntity = NULL;
}

///////////////////////////////////////////////////////////////////////////////
