/// @file   TamyEditor/SelectionManagerListener.h
/// @brief  selection manager listener interface
#pragma once


///////////////////////////////////////////////////////////////////////////////

class Entity;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instance will be informed what entities are currently selected through
 * this interface.
 */
class SelectionManagerListener
{
public:
   virtual ~SelectionManagerListener() {}

   /**
    * Called when an entity is selected.
    *
    * @param entity
    */
   virtual void onEntitySelected( Entity& entity ) = 0;

   /**
    * Called when an entity is deselected.
    *
    * @param entity
    */
   virtual void onEntityDeselected( Entity& entity ) = 0;
};


///////////////////////////////////////////////////////////////////////////////
