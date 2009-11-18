#ifndef _PROPERTIES_H
#error "This file can only be included from Properties.h"
#else

#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template <typename T>
void Properties::add(const std::string& id, T& propVal)
{
   PropsMap::const_iterator it = m_properties.find(id);
   if (it != m_properties.end())
   {
      throw std::runtime_error("Cannot override an existing property");
   }

   m_properties.insert(std::make_pair(id, new TProperty<T>(propVal)));
   m_names.insert(id);
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
typename TProperty<T>& Properties::get(const std::string& id)
{
   PropsMap::iterator it = m_properties.find(id);
   if (it == m_properties.end())
   {
      throw std::runtime_error(std::string("Property ") + id + " is not defined");
   }

   Property* genericProp = it->second;
   TProperty<T>* prop = dynamic_cast<TProperty<T>*> (genericProp);
   if (prop == NULL)
   {
      throw std::runtime_error(std::string("Property ") + id + " has different type.");
   }

   return *prop;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PROPERTIES_H