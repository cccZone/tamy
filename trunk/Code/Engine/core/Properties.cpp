#include "core\Properties.h"
#include "core\Serializer.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

Properties::Properties(const std::string& entityClassName)
: m_entityClassName(entityClassName)
{
}

///////////////////////////////////////////////////////////////////////////////

Properties::~Properties()
{
   for (PropsMap::iterator it = m_properties.begin();
      it != m_properties.end(); ++it)
   {
      delete it->second;
   }
   m_properties.clear();
}

///////////////////////////////////////////////////////////////////////////////

const std::string& Properties::getClassName() const
{
   return m_entityClassName;
}

///////////////////////////////////////////////////////////////////////////////

bool Properties::has(const std::string& id) const
{
   PropsMap::const_iterator it = m_properties.find(id);
   return (it != m_properties.end());
}

///////////////////////////////////////////////////////////////////////////////

Property& Properties::get(const std::string& id)
{
   PropsMap::iterator it = m_properties.find(id);
   if (it == m_properties.end())
   {
      throw std::runtime_error(std::string("Property ") + id + " is not defined");
   }

   return *(it->second);
}

///////////////////////////////////////////////////////////////////////////////

const Properties::Names& Properties::getNames() const
{
   return m_names;
}

///////////////////////////////////////////////////////////////////////////////

void Properties::save(Serializer& serializer)
{
   unsigned int propertiesCount = m_properties.size();
   serializer << propertiesCount;

   std::string name;
   for (PropsMap::iterator it = m_properties.begin(); 
        it != m_properties.end();
        ++it)
   {
      name = it->first;
      serializer << name;
      it->second->serialize(serializer);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Properties::load(Serializer& serializer)
{
   unsigned int propertiesCount = 0;
   serializer << propertiesCount;

   std::string name;
   for (unsigned int i = 0; i < propertiesCount; ++i)
   {
      serializer << name;
      PropsMap::iterator it = m_properties.find(name);
      if (it != m_properties.end())
      {
         it->second->serialize(serializer);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////