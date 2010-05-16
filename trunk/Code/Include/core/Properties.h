#ifndef _PROPERTIES_H
#define _PROPERTIES_H

/// @file   core\Properties.h
/// @brief  entity properties management mechanism

#include <set>
#include <map>
#include <string>
#include "core\Property.h"


///////////////////////////////////////////////////////////////////////////////

class Serializer;

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
   std::string m_entityClassName;

   PropsMap m_properties;
   Names m_names;

public:
   /**
    * Constructor.
    *
    * @param entityClassName  name of the class the properties represent.
    */
   Properties(const std::string& entityClassName);
   ~Properties();

   /**
    * Returns the name of the class the properties represent.
    *
    * @return     class name
    */
   const std::string& getClassName() const;

   /**
    * This method allows to define a new property. All properties
    * MUST have unique ids assigned.
    *
    * @throws std::runtime_error if we try to override an existing property 
    *                            name
    * @param T          type of the property we want to define
    * @param propValue  variable that holds the property value and that will
    *                   be edited by this property
    * @param id         unique name for the property (good rule of thumb is to
    *                   name the properties after the variables they refer to)
    *
    * @return a reference to the newly created property
    */
   template <typename T>
   Property& add( T& propVal, const std::string& id );

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

   /**
    * Saves the properties using the specified serializer.
    *
    * @param serializer
    */
   void save(Serializer& serializer);

   /**
    * Loads the properties using the specified serializer.
    *
    * @param serializer
    */
   void load(Serializer& serializer);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Properties.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _PROPERTIES_H
