/// @file   core/ReflectionObject.h
/// @brief  object with RTTI and serialization support
#ifndef _REFLECTION_OBJECT_H
#define _REFLECTION_OBJECT_H

#include "core/types.h"
#include <string>
#include "core/ReflectionMacros.h"
#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class SerializableReflectionType;
class ReflectionPropertiesView;
class ReflectionProperty;
class ReflectionObjectChangeListener;

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
   typedef std::string                                UniqueId;
   UniqueId                                           m_uniqueId;

private:
   Array< ReflectionObjectChangeListener* >           m_listener;

   // a counter that counts the number of references to this object
   int                                                m_referencesCounter;

public:
   virtual ~ReflectionObject();

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
    * Attaches a new object listener.
    *
    * @param listener
    */
   void attachListener( ReflectionObjectChangeListener& listener );

   /**
    * Attaches a new object listener.
    *
    * @param listener
    */
   void detachListener( ReflectionObjectChangeListener& listener );

   /**
    * Notifies that the value of one of this object's properties has changed.
    *
    * @param propertyName     name of the property that's changed
    */
   void notifyPropertyChange( const std::string& propertyName );

   /**
    * Called once an object and all of its dependencies is fully loaded.
    *
    * CAUTION: the order in which those methods will be called on dependent objects
    * is UNDEFINED. Therefore it's not a good place to be accessing any methods on the
    * referenced objects, because those objects onObjectLoaded method may have not yet been called !
    *
    * If you need to do that, Resurce::onResourceLoaded is the place to do it as that method
    * will be called once ALL dependencies have been restored and all objects have their onObjectLoaded method called.
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

   // -------------------------------------------------------------------------
   // References counting
   // -------------------------------------------------------------------------
   /**
    * Checks how many references to the object are there.
    */
   inline int getReferencesCount() const { return m_referencesCounter; }

   /**
    * Adds a new reference to the object.
    */
   void addReference();

   /**
    * Removes a reference to the object.
    * 
    * When the references counter reaches 0, the object will be automatically deleted.
    */
   void removeReference();

protected:
   /**
    * Constructor.
    *
    * @param uniqueId   ( optional )
    */
   ReflectionObject( const char* uniqueId = NULL );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Casts an object from one type to another. The types need to share a common hierarchy.
 */
template< typename T >
T* DynamicCast( ReflectionObject* obj );

///////////////////////////////////////////////////////////////////////////////

/**
 * A macro that safely deletes a ReflectionObject instance
 */
#define SAFE_DELETE( object ) \
   if ( object ) \
   { \
      object->removeReference(); \
      object = NULL; \
   }

///////////////////////////////////////////////////////////////////////////////

#include "core/ReflectionObject.inl"

///////////////////////////////////////////////////////////////////////////////

// an include that facilitates the use of ReflectionObject, so that we don't
// have to add similar includes to each file that contains a class that
// uses the ReflectionObject
#include "core/ReflectionType.h"

///////////////////////////////////////////////////////////////////////////////

#endif // _REFLECTION_OBJECT_H
