#pragma once

/// @file   TamyEditor\EditEntityCommand.h
/// @brief  command for selecting& editing an entity from a scene

#include "core.h"
#include "SceneQueries.h"
#include "SceneQuery.h"
#include "InputCommand.h"
#include "core\Array.h"
#include "SelectionManager.h"
#include "ManualEditionModes.h"

///////////////////////////////////////////////////////////////////////////////

class Entity;
class SelectionManager;
class UserInputController;
class TimeControllerTrack;
class EntityManualEditor;
class TamyEditor;

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
   class State
   {
   public:
      virtual ~State() {}

      virtual void edit( float timeElapsed ) = 0;
      virtual void start() = 0;
      virtual void stop() = 0;
   };

   class NoEdition : public State
   {
   private:
      EditEntityCommand&   m_mgr;

   public:
      NoEdition( EditEntityCommand& mgr ) : m_mgr( mgr ) {}
      void edit( float timeElapsed ) {}
      void start() { m_mgr.m_activeState = m_mgr.m_editionStart; }
      void stop() {}
   };
   friend class NoEdition;

   class EditionStart : public State
   {
   private:
      EditEntityCommand&   m_mgr;

   public:
      EditionStart( EditEntityCommand& mgr ) : m_mgr( mgr ) {}
      void edit( float timeElapsed ) 
      { 
         m_mgr.startEdition(); 
         m_mgr.m_activeState = m_mgr.m_editionInProgress;
      }
      void start() {}
      void stop() { m_mgr.m_activeState = m_mgr.m_editionStop; }
   };
   friend class EditionStart;

   class EditionInProgress : public State
   {
   private:
      EditEntityCommand&   m_mgr;

   public:
      EditionInProgress( EditEntityCommand& mgr ) : m_mgr( mgr ) {}
      void edit( float timeElapsed ) { m_mgr.edit( timeElapsed ); }
      void start() {}
      void stop() { m_mgr.m_activeState = m_mgr.m_editionStop; }
   };
   friend class EditionStart;

   class EditionStop : public State
   {
   private:
      EditEntityCommand&   m_mgr;

   public:
      EditionStop( EditEntityCommand& mgr ) : m_mgr( mgr ) {}
      void edit( float timeElapsed ) 
      { 
         m_mgr.stopEdition(); 
         m_mgr.m_activeState = m_mgr.m_noEdition;
      }
      void start() { m_mgr.m_activeState = m_mgr.m_editionStop; }
      void stop() {}
   };
   friend class EditionStart;

private:
   TamyEditor&          m_servicesMgr;
   SceneQueries*        m_scene;
   SelectionManager*    m_selectionMgr;
   UserInputController* m_uic;

   D3DXVECTOR2          m_queriedPos;

   NodeEditionMode      m_editionMode;
   EntityManualEditor*  m_editor;

   // -------------------------------------------------------------------------
   // Edition states
   // -------------------------------------------------------------------------
   State*               m_activeState;
   State*               m_editionStart;
   State*               m_editionInProgress;
   State*               m_editionStop;
   State*               m_noEdition;

public:
   /**
    * Constructor.
    *
    * @param servicesMgr
    * @param timeTrack
    */
   EditEntityCommand( TamyEditor& servicesMgr, TimeControllerTrack& timeTrack );
   ~EditEntityCommand();

   /**
    * Changes parameters of the edited object.
    */
   void update( float timeElapsed );

   /**
    * Returns an instance of the services manager.
    */
   inline TamyEditor& getServicesMgr() const { return m_servicesMgr; }

   // -------------------------------------------------------------------------
   // Edition modes settings
   // -------------------------------------------------------------------------
   /**
    * Sets a new entity edition mode.
    *
    * @param mode
    */
   void setNodeEditionMode( NodeEditionMode mode );

   /**
    * Sets a new entity edition mode.
    */
   NodeEditionMode getNodeEditionMode() const;

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

private:
   // -------------------------------------------------------------------------
   // State operation
   // -------------------------------------------------------------------------
   void startEdition();
   void edit( float timeElapsed );
   void stopEdition();
};

///////////////////////////////////////////////////////////////////////////////
