/// @file   core\ReflectionProperty.h
/// @brief  generic property interface
#ifndef REFLECTION_PROPERTY_H
#define REFLECTION_PROPERTY_H

#include <string>
#include <map>
#include <vector>
#include "core\types.h"


///////////////////////////////////////////////////////////////////////////////

class ReflectionObject;
class ReflectionProperty;
class ReflectionPropertiesView;
class ReflectionType;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a generic property interface. It will allow to store properties
 * of various data types in a uniform manner in all sorts of collections.
 *
 * A property can be observed - its observers will get notified about changes 
 * the property undergoes.
 */
class ReflectionProperty
{
private:
   ReflectionObject*                m_observer;
   std::string                      m_name;
   std::string                      m_label;
   bool                             m_canBeEdited;
   bool                             m_canBeSaved;

public:
   virtual ~ReflectionProperty() {}

   /**
    * Sets the property name.
    *
    * @param name
    * @param label
    * @param canBeEdited
    * @param canBeSaved
    */
   inline void setParams( const std::string& name, const std::string& label, bool canBeEdited, bool canBeSaved );

   /**
    * This method returns the name assigned to this property.
    *
    * @return  property name
    */
   inline const std::string& getName() const;

   /**
    * This method returns a label under which the property should
    * be advertised (in editor i.e.)
    *
    * @return  property label
    */
   inline const std::string& getLabel() const;

   /**
    * Tells whether we want to edit this property or not.
    */
   inline bool canBeEdited() const;

   /**
    * Sets a new value of the property.
    *
    * @param val
    */
   virtual void set( void* val ) = 0;

   /**
    * Allows to edit the property contents by a property editor.
    */
   virtual void* edit() = 0;

   /**
    * Tells if the property can aggregate other properties, or is it a leaf.
    */
   virtual bool isComposite() const = 0;

   /**
    * Returns an instance of ReflectionObject this property belongs to.
    */
   inline ReflectionObject* getHostObject() const { return m_observer; }

   // -------------------------------------------------------------------------
   // Type identification mechanism.
   // -------------------------------------------------------------------------
   /**
    * Retrieves the virtual (topmost in terms of inheritance hierarchy) class
    * of the element stored in the property.
    *
    * i.e. lets take the following classes hierarchy:
    *       class A {};  class B : public A {};
    *    and property setup:
    *       B* val;
    *       TProperty< A* > property( val );
    *    then:
    *       property.getVirtualClass() == B;
    */
   virtual const ReflectionType& getVirtualClass() const = 0;

   /**
    * Returns the type the property is parametrized with ( static type, but accessed through a specific instance ).
    */
   virtual const ReflectionType& getPropertyClass() const = 0;

   /**
    * Another RTTI method is required:
    *
    * static const ReflectionType& getRTTIClass();
    *
    * It should return the static type of the represented property.
    */

   // -------------------------------------------------------------------------
   // Observation mechanism
   // -------------------------------------------------------------------------
   /**
    * Call this method when the value of the property is about to change ( a great moment
    * to clean up after the old value ).
    */
   void notifyBeforeChange();

   /**
    * Call this method when the value of the property changes.
    */
   void notifyAfterChange();

protected:
   /**
    * Constructor.
    *
    * @param hostObject
    */
   ReflectionProperty( ReflectionObject* hostObject = NULL );

};

///////////////////////////////////////////////////////////////////////////////

/**
 * This specialized property allows to create properties for all types of data.
 */
template< typename T >
class TReflectionProperty : public ReflectionProperty
{
private:
   T*             m_val;

public:
   /**
    * Constructor.
    *
    * @param hostObject
    * @param val
    */
   TReflectionProperty( ReflectionObject* hostObject, T* val );

   // -------------------------------------------------------------------------
   // ReflectionProperty implementation
   // -------------------------------------------------------------------------
   void set( void* val );
   void* edit();
   bool isComposite() const { return false; }

   // -------------------------------------------------------------------------
   // Type identification mechanism implementation.
   // -------------------------------------------------------------------------
   const ReflectionType& getVirtualClass() const;
   const ReflectionType& getPropertyClass() const;
   static const ReflectionType& getRTTIClass();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This specialized property allows to create properties for all types of pointers.
 */
template< typename T >
class TReflectionProperty< T* > : public ReflectionProperty
{
private:
   T** m_val;

public:
   /**
    * Constructor.
    *
    * @param hostObject
    * @param val
    */
   TReflectionProperty( ReflectionObject* hostObject, T** val );

   // -------------------------------------------------------------------------
   // ReflectionProperty implementation
   // -------------------------------------------------------------------------
   void set( void* val );
   void* edit();
   bool isComposite() const { return false; }

   // -------------------------------------------------------------------------
   // Type identification mechanism implementation.
   // -------------------------------------------------------------------------
   const ReflectionType& getVirtualClass() const;
   const ReflectionType& getPropertyClass() const;
   static const ReflectionType& getRTTIClass();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A marker interface for the properties that holds arrays of data.
 * It's needed so that we can write generic editors for those properties.
 */
class ReflectionPropertyArray : public ReflectionProperty
{
private:
   std::string             m_emptyStr;

public:
   /**
    * Constructor.
    *
    * @param hostObject
    */
   ReflectionPropertyArray( ReflectionObject* hostObject );
   virtual ~ReflectionPropertyArray() {}

   /**
    * Returns the size of the array.
    */
   virtual uint size() const = 0;

   /**
    * Returns an object the specified array element contains.
    *
    * @param idx     element's index
    */
   virtual ReflectionObject* getElement( uint idx ) = 0;

   // -------------------------------------------------------------------------
   // ReflectionProperty implementation
   // -------------------------------------------------------------------------
   bool isComposite() const { return true; }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This specialized property can hold a vector of pointers
 */
template< typename T >
class TReflectionProperty< std::vector< T* > > : public ReflectionPropertyArray
{
private:
   std::vector<T*>*     m_val;

public:
   /**
    * Constructor.
    *
    * @param hostObject
    * @param val
    */
   TReflectionProperty( ReflectionObject* hostObject, std::vector<T*>* val );

   // -------------------------------------------------------------------------
   // VectorProperty implementation
   // -------------------------------------------------------------------------
   uint size() const;
   ReflectionObject* getElement( uint idx );

   // -------------------------------------------------------------------------
   // ReflectionProperty implementation
   // -------------------------------------------------------------------------
   void set( void* val );
   void* edit();

   // -------------------------------------------------------------------------
   // Type identification mechanism implementation.
   // -------------------------------------------------------------------------
   const ReflectionType& getVirtualClass() const;
   const ReflectionType& getPropertyClass() const;
   static const ReflectionType& getRTTIClass();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Editable version of a property.
 */
template< typename T >
class TEditableReflectionProperty
{
private:
   ReflectionProperty*   m_property;

public:
   TEditableReflectionProperty( ReflectionProperty* property );
   ~TEditableReflectionProperty();

   /**
    * This method returns a label under which the property should
    * be advertised (in editor i.e.)
    *
    * @return  property label
    */
   const std::string& getLabel() const;

   /**
    * Sets new property value.
    *
    * @param val     new property value
    */
   void set( const T& val );

   /**
    * Returns current property value (const version).
    *
    * @return        property value.
    */
   const T& get() const;

   /**
    * Returns current property value.
    *
    * @return        property value.
    */
   T& get();

   /**
    * Returns the type the property is parametrized with.
    */
   const ReflectionType& getType() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ReflectionProperty.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // REFLECTION_PROPERTY_H
