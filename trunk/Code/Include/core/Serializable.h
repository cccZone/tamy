#ifndef _SERIALIZABLE_H
#define _SERIALIZABLE_H

/// @file   core\Serializable.h
/// @brief  interface marking things that can be serialized

#include "core\ClassesRegistry.h"
#include "core\Class.h"


///////////////////////////////////////////////////////////////////////////////

class Serializer;

///////////////////////////////////////////////////////////////////////////////

class Serializable
{
public:
   virtual ~Serializable() {}

   /**
    * The method allows to save the state of serializable object.
    *
    * @param serializer  serializer that will take care of writing
    *                    stuff to an external storage
    */
   inline void save(Serializer& serializer);

   /**
    * The method restores the state of serializable object from 
    * an external storage.
    *
    * @param serializer  serializer that will take care of reading
    *                    stuff from an external storage
    */
   template <typename Derived>
   static Derived* load(Serializer& serializer);

   /**
    * Returns an id uniquely identifying the serialized class.
    */
   virtual int getClassID() const = 0;

   // -------------------------------------------------------------------------
   // State serialization implementation
   // -------------------------------------------------------------------------
   /**
    * Method called when an object is being saved - it should
    * contain implementation specific state saving code.
    *
    * @param serializer  serializer that will take care of writing
    *                    stuff to an external storage
    */
   virtual void onSave(Serializer& serializer) = 0;

   /**
    * Method called when an object is being loaded - it should
    * contain implementation specific state saving code.
    *
    * @param serializer  serializer that will take care of writing
    *                    stuff to an external storage
    */
   virtual void onLoad(Serializer& serializer) = 0;

   // -------------------------------------------------------------------------
   // Notifications
   // -------------------------------------------------------------------------
   /**
    * Called once an object is fully loaded.
    */
   virtual void onObjectLoaded() {}

   /**
    * Called before an object is serialized.
    */
   virtual void onObjectPreSave() {}
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Serializable.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _SERIALIZABLE_H
