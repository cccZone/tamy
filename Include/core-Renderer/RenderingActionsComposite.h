#pragma once

/// @file   core-Renderer\RenderingActionsComposite.h
/// @brief  a composite of rendering actions

#include "core-Renderer\RenderingAction.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * A composite capable of holding and executing multiple
 * rendering actions as if it was one action
 */
class RenderingActionsComposite : public RenderingAction
{
private:
   Array<RenderingAction*> m_actions;

public:
   ~RenderingActionsComposite();

   /**
    * Adds a new action to the composite
    *
    * @param action     action we want to have running in the composite
    */
   void addAction(RenderingAction* action);

   void onRenderingBegin(Renderable& renderable, RendererImpl& impl);

   void onRenderingEnd(Renderable& renderable, RendererImpl& impl);
};

///////////////////////////////////////////////////////////////////////////////


