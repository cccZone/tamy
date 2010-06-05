#pragma once

/// @file   TamyEditor\EditEntityCommand.h
/// @brief  command for selecting& editing an entity from a scene

#include "core.h"
#include "SceneQueries.h"
#include "SceneQuery.h"
#include "InputCommand.h"
#include "core\Array.h"
#include "SelectionManager.h"


///////////////////////////////////////////////////////////////////////////////

class Entity;
class SelectionManager;
class UserInputController;
class TimeControllerTrack;
class Camera;
class EntityManualEditor;

///////////////////////////////////////////////////////////////////////////////

enum EntityEditionMode
{
   EEM_TRANSLATE,
   EEM_ROTATE,
   EEM_SCALE
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command for selecting entity from a scene.
 */
class EditEntityCommand :  public InputCommand, 
                           public SceneQuery, 
                           public SelectionManagerListener,
                           public GenericFactory< Entity, EntityManualEditor >
{

private:
   SceneQueries&        m_scene;
   SelectionManager&    m_selectionMgr;
   UserInputController& m_uic;
   Camera&              m_camera;

   D3DXVECTOR2          m_queriedPos;
   bool                 m_edition;

   EntityEditionMode    m_editionMode;
   EntityManualEditor*  m_editor;

public:
   /**
    * Constructor.
    *
    * @param scene         scene we want to query
    * @param selectionMgr  selection manager
    * @param uic           user input controller
    * @param camera        camera rendering the scene
    * @param timeTrack     track that will keep updating the command
    */
   EditEntityCommand( SceneQueries& scene, 
                      SelectionManager& selectionMgr, 
                      UserInputController& uic,
                      Camera& camera,
                      TimeControllerTrack& timeTrack );
   ~EditEntityCommand();

   /**
    * Changes parameters of the edited object.
    */
   void update( float timeElapsed );

   /**
    * Sets a new entity edition mode.
    */
   void setEditionMode( EntityEditionMode mode );

   // -------------------------------------------------------------------------
   // MousePointerCommand implementation
   // -------------------------------------------------------------------------
   void execute( InputState state, const D3DXVECTOR2& mousePos );

   // -------------------------------------------------------------------------
   // SceneQuery implementation
   // -------------------------------------------------------------------------
   const D3DXVECTOR2& getQueriedPosition() const { return m_queriedPos; }
   void setResult( Entity* foundEntity );

   // -------------------------------------------------------------------------
   // SelectionManagerListener implementation
   // -------------------------------------------------------------------------
   void onObjectSelected( Entity& entity );
   void onObjectDeselected( Entity& entity );
};

///////////////////////////////////////////////////////////////////////////////
