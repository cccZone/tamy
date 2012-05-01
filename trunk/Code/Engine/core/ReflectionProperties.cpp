#include "core\ReflectionProperties.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

ReflectionProperties::ReflectionProperties( ReflectionObject& hostObject, const std::string& entityClassName )
   : m_hostObject( hostObject )
   , m_entityClassName(entityClassName)
{
}

///////////////////////////////////////////////////////////////////////////////

ReflectionProperties::~ReflectionProperties()
{
   for ( PropsMap::iterator it = m_properties.begin(); it != m_properties.end(); ++it )
   {
      delete it->second;
   }
   m_properties.clear();
}

///////////////////////////////////////////////////////////////////////////////

const std::string& ReflectionProperties::getClassName() const
{
   return m_entityClassName;
}

///////////////////////////////////////////////////////////////////////////////

bool ReflectionProperties::has(const std::string& id) const
{
   PropsMap::const_iterator it = m_properties.find(id);
   return (it != m_properties.end());
}

///////////////////////////////////////////////////////////////////////////////

ReflectionProperty& ReflectionProperties::get(const std::string& id)
{
   PropsMap::iterator it = m_properties.find(id);
   if (it == m_properties.end())
   {
      throw std::runtime_error(std::string("ReflectionProperty ") + id + " is not defined");
   }

   return *(it->second);
}

///////////////////////////////////////////////////////////////////////////////

const ReflectionProperties::Names& ReflectionProperties::getNames() const
{
   return m_names;
}

///////////////////////////////////////////////////////////////////////////////
