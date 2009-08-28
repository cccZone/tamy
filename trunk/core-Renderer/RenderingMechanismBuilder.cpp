#include "core-Renderer\RenderingMechanismBuilder.h"


///////////////////////////////////////////////////////////////////////////////

RenderingMechanismBuilder::RenderingMechanismBuilder() 
: m_mech(new CompositeRenderingMechanism()) 
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanismBuilder::~RenderingMechanismBuilder() 
{
   delete m_mech;
   m_mech = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingMechanismBuilder::operator()(const RenderingNode& node)
{
   if (node.mech == NULL) {return;}

   
   if (m_sameMechFilter.find(node.mech) == m_sameMechFilter.end())
   {
      m_mech->add(node.mech);
      m_sameMechFilter.insert(node.mech);
   }
  
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanism* RenderingMechanismBuilder::get() 
{
   RenderingMechanism* result = m_mech;
   m_mech = new CompositeRenderingMechanism();

   return result;
}

///////////////////////////////////////////////////////////////////////////////
