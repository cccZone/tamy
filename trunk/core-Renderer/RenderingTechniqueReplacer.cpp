#include "core-Renderer\RenderingTechniqueReplacer.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\RenderingTechnique.h"


///////////////////////////////////////////////////////////////////////////////

RenderingTechniqueReplacer::RenderingTechniqueReplacer(const RenderingTechnique& source, 
                                                       RenderingTechnique& target)
      : m_source(source),
      m_target(target)
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingTechniqueReplacer::visit(AbstractGraphicalNode& node)
{
   if (m_source == node.getTechnique())
   {
      node.setTechnique(m_target);
   }
}

///////////////////////////////////////////////////////////////////////////////
