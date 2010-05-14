#pragma once

/// @file   TamyEditor\SelectionManager.h
/// @brief  Component that manages the scene entities selection

#include "core/Component.h"
#include "tamyeditor.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Entity;
class Effect;

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
class SelectionManager: public Component< TamyEditor >
{
private:
   typedef std::vector< SelectionManagerListener* > Listeners;

private:
   Listeners      m_listeners;

   Entity*        m_selectedEntity;
   Effect*        m_selectionMarker;

public:
   /**
    * Constructor.
    */
   SelectionManager();

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

private:
   void visualizeSelection( Entity* newSelection );
   void notifyEntitySelected( Entity& entity );
   void notifyEntityDeselected( Entity& entity );
};

///////////////////////////////////////////////////////////////////////////////
