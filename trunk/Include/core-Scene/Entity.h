#pragma once

/// @file   core-Scene\Entity.h
/// @brief  object populating a model

#include "core-Scene\ClassesRegistry.h"
#include "core\Serializable.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Models consist of various entities that interact with each other
 * and create the model state.
 */
class Entity : public Serializable
{
private:
   int m_classHandle;

public:
   virtual ~Entity() {}

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

ClassesRegistry& getEntitiesRegistry();

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro in derived entity constructor to initialize the 
 * base Entity class.
 */
#define INIT_ENTITY(DerivedClassName) Entity(getEntitiesRegistry().getHandle<DerivedClassName> ())

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register an entity with the reflection system.
 */
#define DEFINE_ENTITY(EntityClassName)                                        \
class Register##EntityClassName                                               \
{                                                                             \
public:                                                                       \
   Register##EntityClassName()                                                \
   {                                                                          \
      getEntitiesRegistry().define<EntityClassName> ();                       \
   }                                                                          \
};                                                                            \
Register##EntityClassName register##EntityClassName;

///////////////////////////////////////////////////////////////////////////////
