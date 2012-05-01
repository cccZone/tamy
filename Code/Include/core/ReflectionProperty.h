/// @file   core\ReflectionProperty.h
/// @brief  generic property interface
#ifndef REFLECTION_PROPERTY_H
#define REFLECTION_PROPERTY_H

#include <string>
#include <map>
#include <vector>


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
    * Returns the type the property is parametrized with.
    */
   virtual const ReflectionType& getPropertyClass() const = 0;

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
template <typename T>
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
template <typename T>
class TReflectionProperty<T*> : public ReflectionProperty
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

   // -------------------------------------------------------------------------
   // Type identification mechanism implementation.
   // -------------------------------------------------------------------------
   const ReflectionType& getVirtualClass() const;
   const ReflectionType& getPropertyClass() const;
   static const ReflectionType& getRTTIClass();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A marker interface for the properties that hold STL vectors.
 * It's needed so that we can write generic editors for those properties.
 */
class VectorReflectionProperty : public ReflectionProperty
{
private:
   std::string             m_emptyStr;

public:
   virtual ~VectorReflectionProperty() {}

   /**
    * Returns the size of the array.
    */
   virtual unsigned int size() const { return 0; }

   /**
    * Allows to view the properties of the selected item.
    *
    * @param idx     item's index
    * @param view    properties viewer
    */
   virtual void viewProperties( unsigned int idx, ReflectionPropertiesView& view ) {}

   // -------------------------------------------------------------------------
   // ReflectionProperty implementation
   // -------------------------------------------------------------------------
   const std::string& getName() const { return m_emptyStr; }
   const std::string& getLabel() const { return m_emptyStr; }
   void set( void* val ) {}
   void* edit() { return NULL; }

   // -------------------------------------------------------------------------
   // Type identification mechanism implementation.
   // -------------------------------------------------------------------------
   const ReflectionType& getVirtualClass() const;
   const ReflectionType& getPropertyClass() const;
   static const ReflectionType& getRTTIClass();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This specialized property can hold a vector of pointers
 */
template< typename T >
class TReflectionProperty< std::vector< T* > > : public VectorReflectionProperty
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
   unsigned int size() const;
   void viewProperties( unsigned int idx, ReflectionPropertiesView& view );

   // -------------------------------------------------------------------------
   // ReflectionProperty implementation
   // -------------------------------------------------------------------------
   void set( void* val );
   void* edit();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Editable version of a property.
 */
template< typename T >
class TEditableReflectionProperty
{
private:
   ReflectionProperty&   m_property;

public:
   TEditableReflectionProperty( ReflectionProperty& property );
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
