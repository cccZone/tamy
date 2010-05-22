#include "core-Renderer\RenderingView.h"
#include "core-MVC\Entity.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\RenderableRepresentation.h"
#include "core-Renderer\AttributeSorter.h"
#include "core-Renderer\DefaultAttributeSorter.h"


///////////////////////////////////////////////////////////////////////////////

RenderingView::RenderingView()
: m_sorter(new DefaultAttributeSorter())
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingView::~RenderingView()
{
   for (RenderablesMap::iterator it = m_renderables.begin();
      it != m_renderables.end(); ++it)
   {
      RenderableRepresentation* repr = it->second;
      delete repr;
   }
   m_renderables.clear();

   delete m_sorter; m_sorter;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::setAttributeSorter(AttributeSorter* sorter)
{
   delete m_sorter;
   m_sorter = sorter;

   for (RenderablesMap::iterator it = m_renderables.begin();
      it != m_renderables.end(); ++it)
   {
      m_sorter->add(*(it->second));
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::update()
{
   m_sorter->render();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityAdded(Entity& entity)
{
   Renderable* renderable = dynamic_cast<Renderable*> (&entity);
   if (renderable == NULL) 
   {
      return;
   }

   RenderablesMap::iterator it = m_renderables.find(renderable);
   if (it == m_renderables.end())
   {
      RenderableRepresentation* repr = new RenderableRepresentation(*renderable, *this);

      m_renderables.insert(std::make_pair(renderable, repr));
      m_sorter->add(*repr);
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityRemoved(Entity& entity)
{
   Renderable* renderable = dynamic_cast<Renderable*> (&entity);
   if (renderable == NULL) 
   {
      return;
   }

   RenderablesMap::iterator it = m_renderables.find(renderable);
   if (it != m_renderables.end())
   {
      RenderableRepresentation* repr = it->second;
      m_renderables.erase(it);

      m_sorter->remove(*repr);
      delete repr;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::onEntityChanged(Entity& entity)
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::resetContents()
{
   for (RenderablesMap::iterator it = m_renderables.begin();
      it != m_renderables.end(); ++it)
   {
      RenderableRepresentation* repr = it->second;

      m_sorter->remove(*repr);
      delete repr;
   }
   m_renderables.clear();
}

///////////////////////////////////////////////////////////////////////////////
