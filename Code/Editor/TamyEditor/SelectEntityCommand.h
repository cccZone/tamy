#pragma once

/// @file   TamyEditor\SelectEntityCommand.h
/// @brief  command for selecting entity from a scene

#include "SceneQueries.h"
#include "SceneQuery.h"
#include "CameraCommand.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class Entity;
class Camera;
struct Ray;
class SelectionManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * Command for selecting entity from a scene.
 */
class SelectEntityCommand : public CameraCommand, public SceneQuery
{

private:
   SceneQueries&        m_scene;
   SelectionManager&    m_selectionMgr;
   D3DXVECTOR2          m_queriedPos;

public:
   /**
    * Constructor.
    *
    * @param scene         scene we want to query
    * @param selectionMgr  selection manager
    */
   SelectEntityCommand( SceneQueries& scene, 
                        SelectionManager& selectionMgr );

   // -------------------------------------------------------------------------
   // MousePointerCommand implementation
   // -------------------------------------------------------------------------
   void execute( const D3DXVECTOR2& mousePos );

   // -------------------------------------------------------------------------
   // SceneQuery implementation
   // -------------------------------------------------------------------------
   const D3DXVECTOR2& getQueriedPosition() const { return m_queriedPos; }
   void setResult( Entity* foundEntity );
};

///////////////////////////////////////////////////////////////////////////////
