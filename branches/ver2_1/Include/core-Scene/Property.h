#pragma once

/// @file   core-Scene\Property.h
/// @brief  generic property interface

#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a generic property interface. It will allow to store properties
 * of various data types in a uniform manner in all sorts of collections.
 */
class Property
{
public:
   virtual ~Property() {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This specialized property allows to create properties for all types of data.
 */
template <typename T>
class TProperty : public Property
{
private:
   std::string m_name;
   std::string m_label;
   T& m_val;

public:
   TProperty(T& val, 
             const std::string& name, 
             const std::string& label) 
   : m_val(val)
   , m_name(name)
   , m_label(label)
   {}

   /**
    * This method returns the name assigned to this property.
    *
    * @return  property name
    */
   const std::string& getName() const {return m_name;}

   /**
    * This method returns a label under which the property should
    * be advertised (in editor i.e.)
    *
    * @return  property label
    */
   const std::string& getLabel() const {return m_label;}

   /**
    * Sets new property value.
    *
    * @param val     new property value
    */
   void set(const T& val) {m_val = val;}

   /**
    * Returns current property value (const version).
    *
    * @return        property value.
    */
   const T& get() const {return m_val;}

   /**
    * Returns current property value.
    *
    * @return        property value.
    */
   T& get() {return m_val;}
};

///////////////////////////////////////////////////////////////////////////////
