#include "core-Renderer\CompositeRenderingMechanism.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

CompositeRenderingMechanism::~CompositeRenderingMechanism() 
{
   unsigned int count = m_members.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_members[i];
   }
   m_members.clear();
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::add(RenderingMechanism* mechanism)
{
   if (mechanism == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderingMechanism instance");
   }
   m_members.push_back(mechanism);
}

///////////////////////////////////////////////////////////////////////////////

void CompositeRenderingMechanism::render()
{
   unsigned int count = m_members.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_members[i]->render();
   }
}

///////////////////////////////////////////////////////////////////////////////
