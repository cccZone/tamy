#include "core-Renderer\Effect.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\EffectAttribute.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT(Effect, Entity)
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Effect::Effect()
: m_attributes(new Attributes())
{
}

///////////////////////////////////////////////////////////////////////////////

Effect::~Effect() 
{
   for (Attributes::iterator it = m_attributes->begin();
        it != m_attributes->end(); ++it)
   {
      delete *it;
   }
   delete m_attributes; m_attributes = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Effect::addAttribute(EffectAttribute* attribute)
{
   if (attribute == NULL)
   {
      throw std::invalid_argument("NULL pointer instead EffectAttribute instance");
   }

   m_attributes->push_back(attribute);
}

///////////////////////////////////////////////////////////////////////////////

const Effect::Attributes& Effect::getAttributes() const
{
   return *m_attributes;
}

///////////////////////////////////////////////////////////////////////////////

void Effect::render(Geometry& geometry)
{
   unsigned int passesCount = beginRendering();
   for (unsigned int i = 0; i < passesCount; ++i)
   {
      beginPass(i);
      geometry.render();
      endPass(i);
   }
   endRendering();
}

///////////////////////////////////////////////////////////////////////////////
