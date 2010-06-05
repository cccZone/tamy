#include "SelectEntityCommand.h"
#include "core-MVC\Entity.h"
#include "SelectionManager.h"


///////////////////////////////////////////////////////////////////////////////

SelectEntityCommand::SelectEntityCommand( SceneQueries& scene, 
                                          SelectionManager& selectionMgr )
: m_scene( scene )
, m_selectionMgr( selectionMgr )
{
}

///////////////////////////////////////////////////////////////////////////////

void SelectEntityCommand::execute( const D3DXVECTOR2& mousePos )
{
   m_queriedPos = mousePos;
   m_scene.query( *this );
}

///////////////////////////////////////////////////////////////////////////////

void SelectEntityCommand::setResult( Entity* foundEntity )
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
