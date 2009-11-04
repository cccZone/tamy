#include "core-AI\CompositeWorldView.h"


///////////////////////////////////////////////////////////////////////////////

void CompositeWorldView::add(GameWorldView& view)
{
   m_views.push_back(&view);
}

///////////////////////////////////////////////////////////////////////////////

void CompositeWorldView::onEntityAdded(Entity& entity)
{
   for (ViewsList::iterator it = m_views.begin(); it != m_views.end(); ++it)
   {
      (*it)->onEntityAdded(entity);
   }
}

///////////////////////////////////////////////////////////////////////////////

void CompositeWorldView::onEntityRemoved(Entity& entity)
{
   for (ViewsList::iterator it = m_views.begin(); it != m_views.end(); ++it)
   {
      (*it)->onEntityRemoved(entity);
   }
}

///////////////////////////////////////////////////////////////////////////////
