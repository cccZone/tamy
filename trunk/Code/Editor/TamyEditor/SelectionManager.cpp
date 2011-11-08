#include "SelectionManager.h"
#include "core-MVC\Model.h"
#include "SelectionManagerListener.h"


///////////////////////////////////////////////////////////////////////////////

SelectionManager::SelectionManager()
   : m_multipleSelectionEnabled( false )
{
}

///////////////////////////////////////////////////////////////////////////////

SelectionManager::~SelectionManager()
{
   resetContents();
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::selectEntity( Entity& entity )
{
   // check if the entity's not already on our list
   unsigned int count = m_selectedEntities.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_selectedEntities[i] == &entity )
      {
         // yup - we have it
         return;
      }
   }

   if ( m_multipleSelectionEnabled == false && count > 0 )
   {
      // if we're not supporting multiple selection, deselect an entity that's currently selected
      resetSelection();
   }

   m_selectedEntities.push_back( &entity );
   notifyEntitySelected( entity );
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::deselectEntity( Entity& entity )
{
   unsigned int count = m_selectedEntities.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_selectedEntities[i] == &entity )
      {
         notifyEntityDeselected( entity );
         m_selectedEntities.erase( m_selectedEntities.begin() + i );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::resetSelection()
{
   unsigned int count = m_selectedEntities.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      notifyEntityDeselected( *m_selectedEntities[i] );
   }
   m_selectedEntities.clear();
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
      m_listeners[i]->onEntitySelected( entity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::notifyEntityDeselected( Entity& entity )
{
   unsigned int count = m_listeners.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_listeners[i]->onEntityDeselected( entity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::onEntityAdded( Entity& entity )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::onEntityRemoved( Entity& entity )
{
   deselectEntity( entity );
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::onEntityChanged( Entity& entity )
{
   // nothing to do here
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::resetContents()
{
   m_selectedEntities.clear();
}

///////////////////////////////////////////////////////////////////////////////
