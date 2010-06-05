#include "EditEntityCommand.h"
#include "core-MVC\Entity.h"
#include "SelectionManager.h"
#include "core-AppFlow.h"
#include "core-Renderer.h"
#include "EntityManualEditor.h"


// editors
#include "NodeManualEditor.h"


///////////////////////////////////////////////////////////////////////////////

EditEntityCommand::EditEntityCommand( SceneQueries& scene, 
                                      SelectionManager& selectionMgr, 
                                      UserInputController& uic,
                                      Camera& camera,
                                      TimeControllerTrack& timeTrack )
: m_scene( scene )
, m_selectionMgr( selectionMgr )
, m_uic( uic )
, m_camera( camera )
, m_edition( false )
, m_editor( NULL )
{
   timeTrack.add( new TTimeDependent< EditEntityCommand >( *this ) );

   // attach self to observe selected objects
   m_selectionMgr.attach( *this );

   // associate editors
   associateAbstract< SpatialEntity, NodeManualEditor >();
}

///////////////////////////////////////////////////////////////////////////////

EditEntityCommand::~EditEntityCommand()
{
   // detach self from the selection manager
   m_selectionMgr.detach( *this );

   // cleanup
   delete m_editor;
   m_editor = NULL;
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
         m_scene.query( *this );
         break;
      }

   case IS_HELD:
      {
         // get ready for the edition - engage relative mouse movement mode
         m_uic.setRelativeMouseMovement( true );
         m_edition = true;
         return;
      }

   case IS_RELEASED:
      {
         // return to the regular mouse movement mode
         m_uic.setRelativeMouseMovement( false );
         m_edition = false;
         return;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::setResult( Entity* foundEntity )
{
   if ( !foundEntity ) 
   {
      m_selectionMgr.resetSelection();
   }
   else
   {
      m_selectionMgr.selectObject( *foundEntity );
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::setEditionMode( EntityEditionMode mode )
{
   m_editionMode = mode;
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::update( float timeElapsed )
{
   // we need to be in the edition mode and have an entity selected in order
   // to edit its params
   if ( !m_edition || !m_editor )
   {
      return;
   }

   // calculate the parameter change factor ( relative to the camera orientation )
   float rotationSpeed = 0.1f * timeElapsed;
   D3DXVECTOR2 mouseSpeed = m_uic.getMouseSpeed() * rotationSpeed;
   m_editor->edit( mouseSpeed, m_camera );
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
   }
}

///////////////////////////////////////////////////////////////////////////////

void EditEntityCommand::onObjectDeselected( Entity& entity )
{
   delete m_editor;
   m_editor = NULL;
}

///////////////////////////////////////////////////////////////////////////////
