#include "core-Renderer\RenderingActionsComposite.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

RenderingActionsComposite::~RenderingActionsComposite()
{
   unsigned int actionsCount = m_actions.size();
   for (unsigned int i = 0; i < actionsCount; ++i)
   {
      delete m_actions[i];
   }
   m_actions.clear();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingActionsComposite::addAction(RenderingAction* action)
{
   if (action == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderingAction instance");
   }
   m_actions.push_back(action);
}

///////////////////////////////////////////////////////////////////////////////

void RenderingActionsComposite::onRenderingBegin(Renderable& renderable, 
                                                 RendererImpl& impl)
{
   unsigned int actionsCount = m_actions.size();
   for (unsigned int i = 0; i < actionsCount; ++i)
   {
      m_actions[i]->onRenderingBegin(renderable, impl);
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingActionsComposite::onRenderingEnd(Renderable& renderable, 
                                               RendererImpl& impl)
{
   unsigned int actionsCount = m_actions.size();
   for (unsigned int i = 0; i < actionsCount; ++i)
   {
      m_actions[i]->onRenderingEnd(renderable, impl);
   }
}

///////////////////////////////////////////////////////////////////////////////
