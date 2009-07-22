#include "core-Renderer\RenderingTargetsPolicyProxy.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

RenderingTargetsPolicyProxy::RenderingTargetsPolicyProxy()
      : m_defaultPolicy(new SettableRenderingTargetsPolicy()),
      m_currentPolicy(m_defaultPolicy)
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingTargetsPolicyProxy::~RenderingTargetsPolicyProxy()
{
   m_currentPolicy = NULL;

   delete m_defaultPolicy;
   m_defaultPolicy = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingTargetsPolicyProxy::setPolicy(RenderingTargetsPolicy& policy)
{
   m_currentPolicy = &policy;
}

///////////////////////////////////////////////////////////////////////////////

unsigned int RenderingTargetsPolicyProxy::setTargets(unsigned int passIdx)
{
   return m_currentPolicy->setTargets(passIdx);
}

///////////////////////////////////////////////////////////////////////////////

unsigned int RenderingTargetsPolicyProxy::getDefinedPassesCount() const
{
   return m_currentPolicy->getDefinedPassesCount();
}

///////////////////////////////////////////////////////////////////////////////
