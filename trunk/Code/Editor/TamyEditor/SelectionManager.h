#pragma once

/// @file   TamyEditor\SelectionManager.h
/// @brief  Component that manages the scene entities selection

#include "core/Component.h"
#include "core-MVC\ModelView.h"
#include "tamyeditor.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Entity;
class Model;
class SelectionRenderingPass;
class ResourcesManager;
class Camera;
class Gizmo;

///////////////////////////////////////////////////////////////////////////////

class SelectionManagerListener
{
public:
   virtual ~SelectionManagerListener() {}

   virtual void onObjectSelected( Entity& entity ) = 0;

   virtual void onObjectDeselected( Entity& entity ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Component that manages the scene entities selection.
 */
class SelectionManager: public Component< TamyEditor >, 
                        public ModelView
{
private:
   typedef std::vector< SelectionManagerListener* >                     Listeners;

private:
   Listeners                           m_listeners;

   Entity*                             m_selectedEntity;
   SelectionRenderingPass*             m_renderingPass;
   Gizmo*                              m_gizmo;

   Model*                              m_observedScene;

   ResourcesManager*                   m_resMgr;
   Camera*                             m_camera;

public:
   /**
    * Constructor.
    */
   SelectionManager();
   ~SelectionManager();

   /**
    * The method renders the selected objects on screen.
    */
   void render();

   // -------------------------------------------------------------------------
   // An interface for the representations
   // -------------------------------------------------------------------------
   /**
    * Returns a reference to the resources manager instance the representations
    * should reference to get the resources they need.
    */
   inline ResourcesManager& getResourcesManager() const { return *m_resMgr; }

   /**
    * Returns an instance of the camera representations should use to render 
    * themselves.
    */
   inline Camera& getCamera() const { return *m_camera; }

   /**
    * Returns an instance of a gizmo visualizing the selected object's 
    * manipulation mode.
    */
   inline Gizmo& getGizmo() const { return *m_gizmo; }

   // -------------------------------------------------------------------------
   // Selected entities management
   // -------------------------------------------------------------------------
   /**
    * This method allows to select an object for edition.
    *
    * @param entity  entity properties of which we want to edit.
    */
   void selectObject( Entity& entity );

   /**
    * Removes the selected object's properties from the view.
    */
   void resetSelection();

   /**
    * Returns a pointer to the currently selected entity.
    */
   inline const Entity* getSelectedEntity() const { return m_selectedEntity; }

   // -------------------------------------------------------------------------
   // Listeners management
   // -------------------------------------------------------------------------
   /**
    * Attaches a new listener.
    *
    * @param listener
    */
   void attach( SelectionManagerListener& listener );

   /**
    * Detaches a listener.
    *
    * @param listener
    */
   void detach( SelectionManagerListener& listener );

   // -------------------------------------------------------------------------
   // Component initialization
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );
   void onServiceRegistered( TamyEditor& mgr );

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   void onEntityAdded( Entity& entity );
   void onEntityRemoved( Entity& entity );
   void onEntityChanged( Entity& entity );
   void resetContents();

private:
   void notifyEntitySelected( Entity& entity );
   void notifyEntityDeselected( Entity& entity );
};

///////////////////////////////////////////////////////////////////////////////
