#include "core-Renderer\MaterialReplacer.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Material.h"


///////////////////////////////////////////////////////////////////////////////

MaterialReplacer::MaterialReplacer(const Material& source, 
                                   Material& target)
      : m_source(source),
      m_target(target)
{
}

///////////////////////////////////////////////////////////////////////////////

void MaterialReplacer::visit(RenderableNode& node)
{
   if (m_source == node.getMaterial())
   {
      node.setMaterial(m_target);
   }
}

///////////////////////////////////////////////////////////////////////////////
