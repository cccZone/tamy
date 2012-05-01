#ifndef REFLECTION_PROPERTIES_H
#error "This file can only be included from ReflectionProperties.h"
#else

#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template< typename T >
ReflectionProperty& ReflectionProperties::add( T& propVal, const std::string& id )
{
   PropsMap::const_iterator it = m_properties.find(id);
   if (it != m_properties.end())
   {
      throw std::runtime_error("Cannot override an existing property");
   }

   ReflectionProperty* newProperty = new TReflectionProperty<T>( &m_hostObject, &propVal );
   m_properties.insert(std::make_pair(id, newProperty));
   m_names.insert(id);

   return *newProperty;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
typename TReflectionProperty<T>& ReflectionProperties::get(const std::string& id)
{
   PropsMap::iterator it = m_properties.find(id);
   if (it == m_properties.end())
   {
      throw std::runtime_error(std::string("ReflectionProperty ") + id + " is not defined");
   }

   ReflectionProperty* genericProp = it->second;
   TReflectionProperty<T>* prop = dynamic_cast<TReflectionProperty<T>*> (genericProp);
   if (prop == NULL)
   {
      throw std::runtime_error(std::string("ReflectionProperty ") + id + " has different type.");
   }

   return *prop;
}

///////////////////////////////////////////////////////////////////////////////

#endif // REFLECTION_PROPERTIES_H
