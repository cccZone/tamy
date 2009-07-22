#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

RenderingMechanism::RenderingMechanism(RenderingTargetsPolicy* policy)
      : m_ownRenderTargetsPolicy(policy)
{
   if (m_ownRenderTargetsPolicy == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderingTargetsPolicy instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanism::~RenderingMechanism()
{
   delete m_ownRenderTargetsPolicy;
   m_ownRenderTargetsPolicy = NULL;
}

///////////////////////////////////////////////////////////////////////////////
