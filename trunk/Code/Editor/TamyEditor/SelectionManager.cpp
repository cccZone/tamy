#include "SelectionManager.h"
#include "SelectedEntityRepresentation.h"
#include "core-Renderer.h"
#include "core-MVC\Model.h"
#include "Gizmo.h"
#include "SelectionRenderingPass.h"


///////////////////////////////////////////////////////////////////////////////

SelectionManager::SelectionManager()
: m_selectedEntity( NULL )
, m_renderingPass( NULL )
, m_gizmo( NULL )
, m_observedScene( NULL )
, m_resMgr( NULL )
, m_camera( NULL )
{
   // create the rendering pass
   m_renderingPass = new SelectionRenderingPass( *this );
}

///////////////////////////////////////////////////////////////////////////////

SelectionManager::~SelectionManager()
{
   resetContents();

   delete m_gizmo;
   m_gizmo = NULL;

   m_renderingPass = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::initialize( TamyEditor& mgr )
{
   mgr.registerService< SelectionManager >( *this, *this );

   // acquire an instance of the resources manager and a camera
   m_resMgr = &mgr.requestService< ResourcesManager >();
   m_camera = &mgr.requestService< Camera >();

   // create the gizmo
   m_gizmo = new Gizmo( *m_resMgr, *m_camera );

   // register a rendering pass
   CompositeRenderingMechanism& compRenderingMech = mgr.requestService< CompositeRenderingMechanism >();
   compRenderingMech.add( "SelectionManager", m_renderingPass );
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

   // visualize the selection
   m_renderingPass->set( &entity );

   // notify about the selection
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

   // unvisualise the selection
   m_renderingPass->set( NULL );
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
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::notifyEntityDeselected( Entity& entity )
{
   unsigned int count = m_listeners.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_listeners[i]->onObjectDeselected( entity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::onEntityAdded( Entity& entity )
{
}

///////////////////////////////////////////////////////////////////////////////

void SelectionManager::onEntityRemoved( Entity& entity )
{
   // if the entity was selected, remove the selection
   if ( m_selectedEntity == &entity )
   {
      m_selectedEntity = NULL;
      m_renderingPass->set( NULL );
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
   m_renderingPass->set( NULL );
}

///////////////////////////////////////////////////////////////////////////////
