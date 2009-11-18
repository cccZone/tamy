#include "core-Scene\Properties.h"
#include <stdexcept>


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
