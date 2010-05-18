#pragma once

/// @file   TamyEditor\SceneTreeEditor.h
/// @brief  a common interface for an editor operating on the scene entities


///////////////////////////////////////////////////////////////////////////////

class QMenu;
class QAction;
class Class;
class Entity;

///////////////////////////////////////////////////////////////////////////////

class SceneTreeEditor
{
public:
   virtual ~SceneTreeEditor() {}

   /**
    * Creates an action that adds a new entity to the selected 
    * scene object.
    *
    * @param parent        widget running the action
    * @param entityClass   class of the entity we want to create
    */
   virtual void createAddEntityAction( QMenu& parent, 
                                       const Class& entityClass ) = 0;

   /**
    * Creates an action that removes the selected scene object.
    *
    * @param parent        widget running the action
    * @param entity        removed entity
    */
   virtual void createRemoveEntityAction( QMenu& parent, Entity* entity ) = 0;

   /**
    * Creates an action that removes all children of the selected
    * scene object.
    *
    * @param parent        widget running the action
    */
   virtual void createClearAction( QMenu& parent  ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
