#pragma once

/// @file   core-Scene\Entity.h
/// @brief  object populating a model

#include "core-Scene\ClassesRegistry.h"


///////////////////////////////////////////////////////////////////////////////

class ModelSerializer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Models consist of various entities that interact with each other
 * and create the model state.
 */
class Entity
{
protected:
   static ClassesRegistry s_entitiesRegistry;

private:
   int m_classHandle;

public:
   virtual ~Entity() {}

   /**
    * The method allows to save entity's state.
    *
    * @param modelSerializer  serializer that will take care of writing
    *                         stuff to an external storage
    */
   virtual void save(ModelSerializer& serializer) = 0;

   /**
    * The method restores the entity's state from an external storage.
    *
    * @param modelSerializer  serializer that will take care of reading
    *                         stuff from an external storage
    */
   virtual void load(ModelSerializer& serializer) = 0;

   // -------------------------------------------------------------------------
   // Reflection for entities
   // -------------------------------------------------------------------------
   /**
    * Creates an entity using the reflection mechanism.
    *
    * @param entityClassHandle   handle of an entity class we want to create.
    */
   static Entity* create(int entityClassHandle);

   /**
    * Returns a unique reflection class handle assigned to this type of class.
    */
   int getClassHandle() const;

protected:
   /**
    * Constructor.
    */
   Entity(int classHandle = -1);
};

///////////////////////////////////////////////////////////////////////////////

#define INIT_ENTITY(DerrivedClassName) Entity(s_entitiesRegistry.define<DerrivedClassName> ())

///////////////////////////////////////////////////////////////////////////////
