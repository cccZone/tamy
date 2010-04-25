#include "core\Object.h"
#include "core\PropertiesView.h"
#include "core\Assert.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_RTTI(Object)
END_RTTI

///////////////////////////////////////////////////////////////////////////////

Object::Object()
{
}

///////////////////////////////////////////////////////////////////////////////

Object::~Object()
{
   unsigned int count = m_properties.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_properties[i];
   }
   m_properties.clear();
}

///////////////////////////////////////////////////////////////////////////////

void Object::viewProperties(PropertiesView& view)
{
   if (m_properties.empty())
   {
      implementObject();
   }

   view.reset();

   unsigned int count = m_properties.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      view.set(*m_properties[i]);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Object::addProperties(Properties* properties)
{
   if (properties == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a Properties instance");
   }
   m_properties.push_back(properties);
}

///////////////////////////////////////////////////////////////////////////////

void Object::onSave(Serializer& serializer)
{
   if (m_properties.empty())
   {
      implementObject();
   }

   unsigned int count = m_properties.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_properties[i]->save(serializer);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Object::onLoad(Serializer& serializer)
{
   if (m_properties.empty())
   {
      implementObject();
   }

   unsigned int count = m_properties.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_properties[i]->load(serializer);
   }
}

///////////////////////////////////////////////////////////////////////////////

int Object::getClassID() const
{
   return getVirtualClass().getHandle();
}

///////////////////////////////////////////////////////////////////////////////

void Object::onPropertyChanged(Property& property)
{
}

///////////////////////////////////////////////////////////////////////////////
