#ifndef _PROPERTIES_H
#define _PROPERTIES_H

/// @file   core-Scene\Properties.h
/// @brief  entity properties management mechanism

#include <set>
#include <map>
#include <string>
#include "core-Scene\Property.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a properties manager. It allows to add and query for properties
 * of different types.
 */
class Properties
{
public:
   typedef std::set<std::string> Names;
   typedef std::map<std::string, Property*> PropsMap;

private:
   PropsMap m_properties;
   Names m_names;

public:
   ~Properties();

   /**
    * This method allows to define a new property. All properties
    * MUST have unique ids assigned.
    *
    * @throws std::runtime_error if we try to override an existing property 
    *                            name
    * @param T          type of the property we want to define
    * @param id         unique name for the property (good rule of thumb is to
    *                   name the properties after the variables they refer to)
    * @param propValue  variable that holds the property value and that will
    *                   be edited by this property
    */
   template <typename T>
   void add(const std::string& id, T& propVal);

   /**
    * This method allows to verify if a property with the given name exists.
    *
    * @return           'true' if it exists, 'false' otherwise
    */
   bool has(const std::string& id) const;

   /**
    * Allows to obtain a pointer to the typed property instance, allowing
    * to directly call its 'set' and 'get' methods.
    *
    * @param id         requested property name
    * @return           property instance of specific type
    */
   template <typename T>
   typename TProperty<T>& get(const std::string& id);

   /**
    * Gives access to the generic property for that name. At this level
    * we won't know the property's type and we'll need to cast.
    *
    * @param id         requested property name
    * @return           property instance of generic type
    */
   Property& get(const std::string& id);

   /**
    * Returns a set of names of all defined properties.
    *
    * @return           set of property names
    */
   const Names& getNames() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Scene\Properties.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _PROPERTIES_H
