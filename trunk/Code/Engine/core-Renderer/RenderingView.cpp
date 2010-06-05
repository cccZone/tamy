#include "core-Renderer\RenderingView.h"
#include "core-MVC\Entity.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\RenderableRepresentation.h"
#include "core-Renderer\AttributeSorter.h"
#include "core-Renderer\DefaultAttributeSorter.h"


///////////////////////////////////////////////////////////////////////////////

RenderingView::RenderingView()
: m_defaultSorter( new DefaultAttributeSorter() )
, m_sorter( m_defaultSorter )
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

   delete m_defaultSorter; m_defaultSorter = NULL;
   m_sorter = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingView::setAttributeSorter( AttributeSorter& sorter )
{
   for ( RenderablesMap::iterator it = m_renderables.begin();
      it != m_renderables.end(); ++it )
   {
      // remove all renderables from the previous sorter
      m_sorter->remove( *(it->second) );

      // add them to the new one instead
      sorter.add( *(it->second) );
   }

   // memorize pointer to the new sorter
   m_sorter = &sorter;
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
