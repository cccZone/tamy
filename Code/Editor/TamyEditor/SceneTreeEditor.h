#pragma once

/// @file   TamyEditor\SceneTreeEditor.h
/// @brief  a common interface for an editor operating on the scene entities


///////////////////////////////////////////////////////////////////////////////

class QMenu;
class QAction;
class SerializableReflectionType;
class Entity;

///////////////////////////////////////////////////////////////////////////////

class SceneTreeEditor
{
public:
   virtual ~SceneTreeEditor() {}

   /**
    * Adds a new entity to the selected scene object.
    *
    * @param entityClass   class of the entity we want to create
    */
   virtual void addEntity( const SerializableReflectionType& entityClass ) = 0;

   /**
    * Removes an entity from the selected object
    *
    * @param entity        removed entity
    */
   virtual void removeEntity( Entity* entity ) = 0;

   /**
    * Removes all children of the selected scene object.
    */
   virtual void clearEntity() = 0;
};

///////////////////////////////////////////////////////////////////////////////
