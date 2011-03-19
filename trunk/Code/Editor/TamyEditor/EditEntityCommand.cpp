#include "EditEntityCommand.h"
#include "core-MVC\Entity.h"
#include "SelectionManager.h"
#include "core-AppFlow.h"
#include "core-Renderer.h"
#include "EntityManualEditor.h"

// editors
#include "NodeManualEditor.h"
#include "GeometryManualEditor.h"


///////////////////////////////////////////////////////////////////////////////

EditEntityCommand::EditEntityCommand( TamyEditor& servicesMgr, TimeControllerTrack& timeTrack )
: m_servicesMgr( servicesMgr )
, m_editor( NULL )
, m_editionMode( NEM_TRANSLATE )
, m_editionStart( new EditionStart( *this ) )
, m_editionInProgress( new EditionInProgress( *this ) )
, m_editionStop( new EditionStop( *this ) )
, m_noEdition( new NoEdition( *this ) )
, m_activeState( NULL )
{
   m_activeState = m_noEdition;

   // acquire required services
   m_scene = &servicesMgr.requestService< SceneQueries >();
   m_selectionMgr = &servicesMgr.requestService< SelectionManager >();
   m_uic = &servicesMgr.requestService< UserInputController >();

   // register self for regular updates
   timeTrack.add( new TTimeDependent< EditEntityCommand >( *this ) );

   // attach self to observe selected objects
   m_selectionMgr->attach( *this );

   // associate editors
   associateAbstract< SpatialEntity, NodeManualEditor >();
   associateAbstract< Geometry, GeometryManualEditor >();
}

///////////////////////////////////////////////////////////////////////////////

EditEntityCommand::~EditEntityCommand()
{
   // detach self from the selection manager
   m_selectionMgr->detach( *this );

   // cleanup
   delete m_editor;
   m_editor = NULL;

   delete m_editionStart;
   m_editionStart = NULL;

   delete m_editionInProgress;
   m_editionInProgress = NULL;

   delete m_editionStop;
   m_editionStop = NULL;

   delete m_noEdition;
   m_noEdition = NULL;

   m_activeState = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::execute( InputState state, const D3DXVECTOR2& mousePos )
{
   switch( state )
   {
   case IS_PRESSED:
      {
         // setup a scene selection query
         m_queriedPos = mousePos;
         m_scene->query( *this );
         m_activeState->stop();
         break;
      }

   case IS_HELD:
      {
         m_activeState->start();
         return;
      }

   case IS_RELEASED:
      {
         m_activeState->stop();
         return;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::setResult( Entity* foundEntity )
{
   if ( !foundEntity ) 
   {
      m_selectionMgr->resetSelection();
   }
   else
   {
      m_selectionMgr->selectObject( *foundEntity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::setNodeEditionMode( NodeEditionMode mode )
{
   m_editionMode = mode;

   if ( m_editor )
   {
      m_editor->notifyEditModeChange();
   }
}

///////////////////////////////////////////////////////////////////////////////

NodeEditionMode EditEntityCommand::getNodeEditionMode() const
{
   return m_editionMode;
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::update( float timeElapsed )
{
   m_activeState->edit( timeElapsed );
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::onObjectSelected( Entity& entity )
{
   delete m_editor;
   m_editor = NULL;

   m_editor = create( entity );
   if ( m_editor )
   {
      m_editor->initialize( *this );
      m_editor->notifyEditModeChange();
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::onObjectDeselected( Entity& entity )
{
   delete m_editor;
   m_editor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::startEdition()
{
   if ( !m_editor )
   {
      return;
   }

   const Point& pt = m_uic->getMousePos();
   m_editor->startEdition( D3DXVECTOR2 ( pt.x, pt.y ) );
   m_uic->setRelativeMouseMovement( true );
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::edit( float timeElapsed )
{
   if ( !m_editor )
   {
      return;
   }
   
   // calculate the parameter change factor ( relative to the camera orientation )
   float rotationSpeed = 0.1f * timeElapsed;
   D3DXVECTOR2 mouseSpeed = m_uic->getMouseSpeed() * rotationSpeed;
   m_editor->edit( mouseSpeed );
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::stopEdition()
{
   m_uic->setRelativeMouseMovement( false );
}

///////////////////////////////////////////////////////////////////////////////
