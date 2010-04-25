#ifndef _ENTITY_H
#define _ENTITY_H

/// @file   core-MVC\Entity.h
/// @brief  object populating a model

/**
 * Entities management:
 * ---------------------
 * Entities can be organized in hierarchies. Furthermore, they can
 * be added to a model (a single entity may be added to one model
 * at a time).
 *
 * Entities should be designed to be self contained and self initializing.
 * A model can have multiple components attached to it, and 
 * entities gain access to those components - thus they may initialize
 * themselves based on them.
 * 
 * Serialization:
 * ---------------
 * A model can be serialized. What's important is that components added
 * to a model are not taken into consideration during the serialization.
 * This allows to serialize a model that was i.e. using one instance 
 * of a renderer, and deserialize it using another - all the entities
 * will transparently connect to the other one.
 */

#include "core\Object.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Model;

///////////////////////////////////////////////////////////////////////////////

/**
 * Models consist of various entities that interact with each other
 * and create the model state.
 *
 * An entity has various properties. You can put the properties definitions
 * inside a 'registerProperties' method implementation. 
 */
class Entity : public Object
{
   DECLARE_ABSTRACT_CLASS(Entity)

public:
   typedef std::vector<Entity*> Children;

   Entity* m_parent;
   Children m_children;

   Model* m_hostModel;

public:
   virtual ~Entity();

   // -------------------------------------------------------------------------
   // Entity hierarchy management
   // -------------------------------------------------------------------------
   /**
    * Adds a new child to the entity's hierarchy.
    *
    * @param entity    new child entity
    */
   void add(Entity* entity);

   /**
    * This method removes a child from the entity.
    *
    * @param entity     entity we want to remove
    * @param release    this parameter specifies whether the entity
    *                   should be deleted when it's removed.
    */
   void remove(Entity& entity, bool release = true);

   /**
    * Tells whether this entity is attached to another entity.
    */
   inline bool isAttached() const;

   /**
    * Returns the entity's children.
    */
   inline const Children& getEntityChildren() const;

protected:
   /**
    * Constructor.
    */
   Entity();

   // -------------------------------------------------------------------------
   // Model interaction
   // -------------------------------------------------------------------------
   /**
    * This method will inform an entity that it's been added to a model,
    * which from now on is its host and should be informed about
    * any children added to this entity.
    *
    * @param model   host model
    */
   void onAttachToModel(Model& model);

   /**
    * This method will inform an entity that it's being detached
    * from a host model.
    *
    * @param model   former host model.
    */
   void onDetachFromModel(Model& model);

   // -------------------------------------------------------------------------
   // Notifications
   // -------------------------------------------------------------------------
   /**
    * Method called when a child entity is attached
    * to this entity.
    *
    * @param child      child entity that was attached
    */
   virtual void onChildAttached(Entity& child) = 0;

   /**
    * Method called when a child entity is about to be detached
    * from this entity
    *
    * @param child      child entity about to be detached
    */
   virtual void onChildDetached(Entity& child) = 0;

   /**
    * Method called to inform a child entity about its 
    * new parent when it's being attached to it.
    *
    * @param parent     instance of the new parent entity
    */
   virtual void onAttached(Entity& parent) = 0;

   /**
    * Method called to inform a child that it's being
    * detached from its parent.
    *
    * @param parent     entity instance this entity is being detached from
    */
   virtual void onDetached(Entity& parent) = 0;

   /**
    * Method called to inform an entity that it's being attached
    * to a model.
    *
    * @param hostModel     model the entity is being attached to
    */
   virtual void onAttached(Model& hostModel) = 0;

   /**
    * Method called to inform an entity that it's about to be detached
    * from a model.
    *
    * @param hostModel     model the entity is being detached from
    */
   virtual void onDetached(Model& hostModel) = 0;

   friend class Model;
};

///////////////////////////////////////////////////////////////////////////////

#include "core-MVC\Entity.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _ENTITY_H
