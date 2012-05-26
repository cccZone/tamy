/// @file   core/ReflectionObject.h
/// @brief  object with RTTI and serialization support
#ifndef _REFLECTION_OBJECT_H
#define _REFLECTION_OBJECT_H

#include "core/types.h"
#include <string>
#include "core/ReflectionMacros.h"


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
   DECLARE_CLASS()

public:
   // a unique id assigned to each instance
   typedef std::string     UniqueId;
   UniqueId                m_uniqueId;

public:

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

#include "core/ReflectionObject.inl"

///////////////////////////////////////////////////////////////////////////////

// an include that facilitates the use of ReflectionObject, so that we don't
// have to add similar includes to each file that contains a class that
// uses the ReflectionObject
#include "core/ReflectionType.h"

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_OBJECT_H
