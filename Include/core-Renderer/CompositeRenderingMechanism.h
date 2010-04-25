#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * A composite that can execute many rendering mechanisms in a sequential
 * order.
 */
class CompositeRenderingMechanism : public RenderingMechanism
{
private:
   std::vector<RenderingMechanism*> m_members;

public:
   ~CompositeRenderingMechanism();

   /**
    * Adds a new mechanism to the composite.
    *
    * @param mechanism     rendering mechanism we want to run in the composite
    */
   void add(RenderingMechanism* mechanism);

   void render();
};

///////////////////////////////////////////////////////////////////////////////
