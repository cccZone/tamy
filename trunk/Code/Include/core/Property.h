#ifndef _PROPERTY_H
#define _PROPERTY_H

/// @file   core\Property.h
/// @brief  generic property interface

#include <string>
#include <map>
#include <vector>
#include "core\Serializer.h"
#include "core\Class.h"


///////////////////////////////////////////////////////////////////////////////

class Object;
class Property;

///////////////////////////////////////////////////////////////////////////////

class PropertyObserver
{
public:
   virtual ~PropertyObserver() {}

   /**
    * Called when a property value gets changed.
    */
   virtual void onPropertyChanged(Property& property) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a generic property interface. It will allow to store properties
 * of various data types in a uniform manner in all sorts of collections.
 *
 * A property can be observed - its observers will get notified about changes 
 * the property undergoes.
 */
class Property
{
private:
   PropertyObserver* m_observer;

public:
   virtual ~Property() {}

   /**
    * Serializes the property value.
    *
    * @param   serializer
    */
   virtual void serialize(Serializer& serializer) = 0;

   /**
    * This method returns the name assigned to this property.
    *
    * @return  property name
    */
   virtual const std::string& getName() const = 0;

   /**
    * This method returns a label under which the property should
    * be advertised (in editor i.e.)
    *
    * @return  property label
    */
   virtual const std::string& getLabel() const = 0;

   /**
    * Allows to edit the property contents by a property editor.
    */
   virtual void* edit() = 0;

   /**
    * Allows to edit the property contents by a property editor,
    * treating its contents as a pointer to an object
    */
   virtual Object** editPtr() = 0;

   // -------------------------------------------------------------------------
   // Type identification mechanism.
   // -------------------------------------------------------------------------
   virtual Class getVirtualClass() const = 0;

   // -------------------------------------------------------------------------
   // Change observation mechanism
   // -------------------------------------------------------------------------
   void setObserver(PropertyObserver& observer);

protected:
   /**
    * Constructor.
    */
   Property();

   /**
    * Call this method when the value of the property changes.
    */
   void notifyAboutChange();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This specialized property allows to create properties for all types of data.
 */
template <typename T>
class TProperty : public Property
{
private:
   std::string    m_name;
   std::string    m_label;
   T*             m_val;

public:
   TProperty();
   TProperty( T* val, 
              const std::string& name, 
              const std::string& label );

   /**
    * This method returns the name assigned to this property.
    *
    * @return  property name
    */
   const std::string& getName() const;

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
   void set(const T& val);

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

   // -------------------------------------------------------------------------
   // Property implementation
   // -------------------------------------------------------------------------
   void serialize(Serializer& serializer);
   void* edit();
   Object** editPtr() { return NULL; }

   // -------------------------------------------------------------------------
   // Type identification mechanism implementation.
   // -------------------------------------------------------------------------
   Class getVirtualClass() const;
   static Class getRTTIClass();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This specialized property allows to create properties for all types of pointers.
 */
template <typename T>
class TProperty<T*> : public Property
{
private:
   std::string m_name;
   std::string m_label;
   T** m_val;

public:
   TProperty();
   TProperty(T** val, 
             const std::string& name, 
             const std::string& label);

   /**
    * This method returns the name assigned to this property.
    *
    * @return  property name
    */
   const std::string& getName() const;

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
   void set(const T* val);

   /**
    * Returns current property value (const version).
    *
    * @return        property value.
    */
   const T* get() const;

   /**
    * Returns current property value.
    *
    * @return        property value.
    */
   T* get();

   // -------------------------------------------------------------------------
   // Property implementation
   // -------------------------------------------------------------------------
   void serialize(Serializer& serializer);
   void* edit();
   Object** editPtr();

   // -------------------------------------------------------------------------
   // Type identification mechanism implementation.
   // -------------------------------------------------------------------------
   Class getVirtualClass() const;
   static Class getRTTIClass();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A marker interface for the properties that hold STL vectors.
 * It's needed so that we can write generic editors for those properties.
 */
class VectorProperty : public Property
{
public:
   virtual ~VectorProperty() {}

   /**
    * This method returns the vector's size.
    */
   virtual unsigned int size() const = 0;

   /**
    * Retrieves a value stored under the specified index.
    *
    * @param idx     index of the property
    */
   virtual Object* get(unsigned int idx) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This specialized property can hold a vector of pointers
 */
template< typename T >
class TProperty< std::vector< T* > > : public VectorProperty
{
private:
   class RTTITypeInit
   {
   public:
      RTTITypeInit();
      void realize() {};
   };

private:
   static Class         s_class;
   static RTTITypeInit  s_typeRegistrationTool;

   std::string          m_name;
   std::string          m_label;
   std::vector<T*>*     m_val;

public:
   TProperty(std::vector<T*>* val, 
             const std::string& name, 
             const std::string& label);

   /**
    * This method returns the name assigned to this property.
    *
    * @return  property name
    */
   const std::string& getName() const;

   // -------------------------------------------------------------------------
   // VectorProperty implementation
   // -------------------------------------------------------------------------
   const std::string& getLabel() const;
   unsigned int size() const;
   Object* get( unsigned int idx );

   // -------------------------------------------------------------------------
   // Property implementation
   // -------------------------------------------------------------------------
   void serialize( Serializer& serializer );
   void* edit();
   Object** editPtr() { return NULL; }

   // -------------------------------------------------------------------------
   // Type identification mechanism implementation.
   // -------------------------------------------------------------------------
   Class getVirtualClass() const;
   static Class getRTTIClass();
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Property.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _PROPERTY_H
