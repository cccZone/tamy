#include "core-Renderer\DefaultAttributeSorter.h"
#include "core-Renderer\Renderable.h"


///////////////////////////////////////////////////////////////////////////////

void DefaultAttributeSorter::add(Attributed& object)
{
   m_objects.push_back(&object);
}

///////////////////////////////////////////////////////////////////////////////

void DefaultAttributeSorter::remove(Attributed& object)
{
   for (std::vector<Attributed*>::iterator it = m_objects.begin();
        it != m_objects.end(); ++it)
   {
      if (*it == &object)
      {
         m_objects.erase(it);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void DefaultAttributeSorter::render() 
{
   unsigned int count = m_objects.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_objects[i]->render();
   }
}

///////////////////////////////////////////////////////////////////////////////
