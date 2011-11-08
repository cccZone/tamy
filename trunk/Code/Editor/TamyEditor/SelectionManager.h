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
class SelectionManagerListener;

///////////////////////////////////////////////////////////////////////////////

/**
 * Component that manages the scene entities selection.
 */
class SelectionManager : public ModelView
{
private:
   typedef std::vector< SelectionManagerListener* >      Listeners;

private:
   Listeners                                             m_listeners;
   std::vector< Entity* >                                m_selectedEntities;

   bool                                                  m_multipleSelectionEnabled;

public:
   /**
    * Constructor.
    */
   SelectionManager();
   ~SelectionManager();

   /**
    * Toggles multiple selection.
    *
    * @param enable
    */
   inline void enableMultipleSelection( bool enable ) { m_multipleSelectionEnabled = enable; }

   // -------------------------------------------------------------------------
   // Selected entities management
   // -------------------------------------------------------------------------
   /**
    * This method allows to select an entity.
    *
    * @param entity
    */
   void selectEntity( Entity& entity );

   /**
    * Deselects an entity.
    *
    * @param entity
    */
   void deselectEntity( Entity& entity );

   /**
    * Removes the selected object's properties from the view.
    */
   void resetSelection();


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
