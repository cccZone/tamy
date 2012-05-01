/// @file   core\ReflectionProperties.h
/// @brief  entity properties management mechanism
#ifndef REFLECTION_PROPERTIES_H
#define REFLECTION_PROPERTIES_H

#include <set>
#include <map>
#include <string>
#include "core\ReflectionProperty.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a properties manager. It allows to add and query for properties
 * of different types.
 */
class ReflectionProperties
{
public:
   typedef std::set< std::string > Names;
   typedef std::map< std::string, ReflectionProperty* > PropsMap;

private:
   ReflectionObject& m_hostObject;
   std::string       m_entityClassName;

   PropsMap          m_properties;
   Names             m_names;

public:
   /**
    * Constructor.
    *
    * @param hostObject
    * @param entityClassName  name of the class the properties represent.
    */
   ReflectionProperties( ReflectionObject& hostObject, const std::string& entityClassName );
   ~ReflectionProperties();

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
   ReflectionProperty& add( T& propVal, const std::string& id );

   /**
    * This method allows to verify if a property with the given name exists.
    *
    * @return           'true' if it exists, 'false' otherwise
    */
   bool has( const std::string& id ) const;

   /**
    * Allows to obtain a pointer to the typed property instance, allowing
    * to directly call its 'set' and 'get' methods.
    *
    * @param id         requested property name
    * @return           property instance of specific type
    */
   template <typename T>
   typename TReflectionProperty<T>& get( const std::string& id );

   /**
    * Gives access to the generic property for that name. At this level
    * we won't know the property's type and we'll need to cast.
    *
    * @param id         requested property name
    * @return           property instance of generic type
    */
   ReflectionProperty& get( const std::string& id );

   /**
    * Returns a set of names of all defined properties.
    *
    * @return           set of property names
    */
   const Names& getNames() const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ReflectionProperties.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // REFLECTION_PROPERTIES_H
