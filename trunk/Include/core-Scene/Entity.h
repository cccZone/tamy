#pragma once

/// @file   core-Scene\Entity.h
/// @brief  object populating a model

#include "core-Scene\ClassesRegistry.h"
#include "core\Serializable.h"
#include "core-Scene\Properties.h"
#include "core-Scene\Property.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Models consist of various entities that interact with each other
 * and create the model state.
 *
 * An entity has various properties. You can put the properties definitions
 * inside a 'registerProperties' method implementation. 
 */
class Entity : public Serializable
{
private:
   int m_classHandle;
   Properties* m_properties;

public:
   virtual ~Entity();

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

   // -------------------------------------------------------------------------
   // Properties managements
   // -------------------------------------------------------------------------
   /**
    * Gives access to entity properties.
    *
    * @return     entity properties collection
    */
   Properties& properties();

protected:
   /**
    * Constructor.
    */
   Entity(int classHandle = -1);

   /**
    * This method will be called by 'create' method to register
    * the properties of an entity.
    */
   virtual void registerProperties() = 0;
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

#define PROPERTY(name, label, type, variable) properties().add<type>(name, label, variable)

///////////////////////////////////////////////////////////////////////////////
