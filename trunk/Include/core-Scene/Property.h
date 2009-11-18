#pragma once

/// @file   core-Scene\Property.h
/// @brief  generic property interface


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
   T& m_val;

public:
   TProperty(T& val) : m_val(val) {}

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
