/// @file   core/ReflectionObject.h
/// @brief  object with RTTI and serialization support
#ifndef _REFLECTION_OBJECT_H
#define _REFLECTION_OBJECT_H

#include "core/types.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class SerializableReflectionType;
class ReflectionPropertiesView;
class ReflectionProperty;

///////////////////////////////////////////////////////////////////////////////

/**
 * Derive your class from this class if you want to be able to use RTTI
 * and serialization features on it.
 */
class ReflectionObject
{
public:
   // a unique id assigned to each instance
   typedef std::string     UniqueId;
   UniqueId                m_uniqueId;

public:
   /**
      * Returns the actual type of a polymorphic object.
      */
   virtual const SerializableReflectionType& getVirtualRTTI() const = 0;

   /**
    * Checks if the type of this instance matches exactly the specified reference type.
    *
    * @param referenceType
    */
   bool isExactlyA( const SerializableReflectionType& referenceType ) const;

   /**
    * Checks if the type of this instance matches exactly the specified reference type.
    *
    * @param ReferenceType
    */
   template< typename ReferenceType >
   bool isExactlyA() const;

   /**
    * Checks if this instance can be safely downcast to the specified reference type.
    *
    * @param referenceType
    */
   bool isA( const SerializableReflectionType& referenceType ) const;

   /**
    * Checks if this instance can be safely downcast to the specified reference type.
    *
    * @param ReferenceType
    */
   template< typename ReferenceType >
   bool isA() const;

   /**
    * Gives the specified view access to the object's properties,
    * so that their values can be edited.
    *
    * @param view
    */
   void viewProperties( ReflectionPropertiesView& view );

   // -------------------------------------------------------------------------
   // Notifications
   // -------------------------------------------------------------------------
   /**
    * Called once an object and all of its dependencies is fully loaded.
    */
   virtual void onObjectLoaded() {}

   /**
    * Called before an object is serialized.
    */
   virtual void onObjectPreSave() {}

   /**
    * Called when a property value's about to change.
    *
    * @param property
    */
   virtual void onPrePropertyChanged( ReflectionProperty& property ) {}

   /**
    * Called after a property value changes.
    */
   virtual void onPropertyChanged( ReflectionProperty& property ) {}

protected:
   /**
    * Constructor.
    *
    * @param uniqueId   ( optional )
    */
   ReflectionObject( const char* uniqueId = NULL ) : m_uniqueId( uniqueId ? uniqueId : "" ) {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Casts an object from one type to another. The types need to share a common hierarchy.
 */
template< typename T >
T* DynamicCast( ReflectionObject* obj );

///////////////////////////////////////////////////////////////////////////////
// MACROS
///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro inside a class's declaration body to indicate that 
 * we want it to be a part of the RTTI system.
 */
#define DECLARE_CLASS()                                                       \
   private:                                                                   \
      static SerializableReflectionType*       s_type;                        \
   public:                                                                    \
      virtual const SerializableReflectionType& getVirtualRTTI() const { return *s_type; } \
      static  const SerializableReflectionType& getStaticRTTI() { return *s_type; } \
      static void setupReflectionType( SerializableReflectionType& type );    \
   private:

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro inside a class's declaration body to indicate that 
 * we want it to be a part of the RTTI system.
 */
#define DECLARE_STRUCT()                                                      \
   private:                                                                   \
      static SerializableReflectionType*       s_type;                        \
   public:                                                                    \
      virtual const SerializableReflectionType& getVirtualRTTI() const { return *s_type; } \
      static  const SerializableReflectionType& getStaticRTTI() { return *s_type; } \
      static void setupReflectionType( SerializableReflectionType& type );    \
   public:

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register a class with the reflection system.
 */
#define BEGIN_OBJECT( ClassType )                                             \
   SerializableReflectionType* ClassType::s_type;                             \
   void ClassType::setupReflectionType( SerializableReflectionType& type )    \
   {                                                                          \
      s_type = &type;                                                         \
      ClassType* objPtr = reinterpret_cast< ClassType* >( NULL );

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this macro to register an abstract class with the reflection system.
 */
#define BEGIN_ABSTRACT_OBJECT( ClassType )                                    \
   SerializableReflectionType* ClassType::s_type;                             \
   void ClassType::setupReflectionType( SerializableReflectionType& type )    \
   {                                                                          \
      s_type = &type;                                                         \
      ClassType* objPtr = reinterpret_cast< ClassType* >( NULL );

///////////////////////////////////////////////////////////////////////////////


/**
 * This macro ends the RTTI type implementation
 */
#define END_OBJECT() }

///////////////////////////////////////////////////////////////////////////////

/**
 * Declares the parent type this type derives from
 */
#define PARENT( ParentClassType )                                             \
   s_type->addBaseType( #ParentClassType );


///////////////////////////////////////////////////////////////////////////////

/**
 * Adds a patching information about one of the type's member fields.
 *
 * @param oldMember     member that used to be there, but was removed
 * @param newMember     member that was put in instead of the 'oldMember'
 */
#define PATCH_MEMBER( oldMember, newMember )                                  \
   s_type->patchMemberField( #oldMember, #newMember );

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro defines a non-editable member field.
 */
#define PROPERTY( type, variable )                                            \
   s_type->addMemberField( new TMemberField< type >( #variable, MEMBER_OFFSET( objPtr, variable ) );

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro defines an editable member field.
 */
#define PROPERTY_EDIT( label, type, variable )                                \
   s_type->addMemberField( ( new TMemberField< type >( #variable, MEMBER_OFFSET( objPtr, variable ) )->setEditable( #label ) );

///////////////////////////////////////////////////////////////////////////////

/**
 * This macro defines an editable member field that will not be saved ( is valid
 * only for one runtime ).
 */
#define PROPERTY_EDIT_RUNTIME( label, type, variable )                        \
   s_type->addMemberField( ( new TMemberField< type >( #variable, MEMBER_OFFSET( objPtr, variable ) )->setEditable( #label )->setRuntimeOnly() );

///////////////////////////////////////////////////////////////////////////////

#include "core/ReflectionObject.inl"

///////////////////////////////////////////////////////////////////////////////

// an include that facilitates the use of ReflectionObject, so that we don't
// have to add similar includes to each file that contains a class that
// uses the ReflectionObject
#include "core/ReflectionType.h"

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_OBJECT_H
