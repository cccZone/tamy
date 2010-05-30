#include "core-Renderer\Renderable.h"
#include "core-Renderer\Effect.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\EffectAttribute.h"
#include "core\Assert.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT(Renderable, SpatialEntity)
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Renderable::Renderable( const std::string& name )
: SpatialEntity( name )
, m_geometry()
, m_visible(true)
{}

///////////////////////////////////////////////////////////////////////////////

Renderable::~Renderable()
{
   m_effects.clear();
   m_attributes.clear();
   m_geometry = NULL;
}

///////////////////////////////////////////////////////////////////////////////

const Renderable::Attributes& Renderable::getAttributes() const 
{
   return m_attributes;
}

///////////////////////////////////////////////////////////////////////////////

// TODO: dynamiczny update atrybutow w widoku renderuj�cym
void Renderable::removeEffect(Effect* effect)
{
   ASSERT(effect != NULL, "NULL pointer instead an Effect instance");

   const Effect::Attributes& attribs = effect->getAttributes();
   for (Effect::Attributes::const_iterator it = attribs.begin();
      it != attribs.end(); ++it)
   {
      Attributes::iterator attribIt = std::find(m_attributes.begin(), m_attributes.end(), *it);
      if (attribIt != m_attributes.end())
      {
         m_attributes.erase(attribIt);
      }
   }

   Effects::iterator effectIt = std::find(m_effects.begin(), m_effects.end(), effect);
   if (effectIt != m_effects.end())
   {
      m_effects.erase(effectIt);
   }
}

///////////////////////////////////////////////////////////////////////////////

void Renderable::onChildAttached(Entity& child)
{
   __super::onChildAttached(child);
   
   Effect* effect = dynamic_cast<Effect*> (&child);
   if (effect != NULL)
   {
      addAttributes( *effect );
      m_effects.push_back( effect );
      return;
   }

   // attach new geometry if and only if the current geometry is empty
   Geometry* geometry = dynamic_cast<Geometry*> (&child);
   if (m_geometry == NULL)
   {
      m_geometry = geometry;
      return;
   }
}

///////////////////////////////////////////////////////////////////////////////

void Renderable::onChildDetached(Entity& child)
{
   __super::onChildDetached(child);

   Effect* effect = dynamic_cast<Effect*> (&child);
   if (effect != NULL)
   {
      removeEffect(effect);
      return;
   }

   Geometry* geometry = dynamic_cast<Geometry*> (&child);
   if ((geometry != NULL) && (m_geometry == geometry))
   {
      m_geometry = NULL;
      return;
   }
}

///////////////////////////////////////////////////////////////////////////////

Renderable::Effects& Renderable::getEffects()
{
   return m_effects;
}

///////////////////////////////////////////////////////////////////////////////

bool Renderable::hasGeometry() const
{
   return m_geometry != NULL;
}

///////////////////////////////////////////////////////////////////////////////

Geometry& Renderable::getGeometry()
{
   return *m_geometry;
}

///////////////////////////////////////////////////////////////////////////////

void Renderable::setVisible(bool visible)
{
   m_visible = visible;
}

///////////////////////////////////////////////////////////////////////////////

bool Renderable::isVisible() const
{
   return m_visible;
}

///////////////////////////////////////////////////////////////////////////////

void Renderable::onPropertyChanged(Property& property)
{
   __super::onPropertyChanged( property );
}

///////////////////////////////////////////////////////////////////////////////

void Renderable::onObjectLoaded()
{
   __super::onObjectLoaded();

   // add attributes of the effects
   for ( Effects::iterator it = m_effects.begin(); it != m_effects.end(); ++it )
   {
      Effect* effect = *it;
      addAttributes( *effect );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Renderable::addAttributes( Effect& effect )
{
   const Effect::Attributes& attribs = effect.getAttributes();
   for (Effect::Attributes::const_iterator it = attribs.begin();
      it != attribs.end(); ++it)
   {
      m_attributes.push_back(*it);
   }
}

///////////////////////////////////////////////////////////////////////////////
