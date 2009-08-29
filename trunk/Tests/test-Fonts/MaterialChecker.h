#pragma once

#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\Material.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialChecker : public NodeVisitor,
                                  public TNodesVisitor<AbstractGraphicalNode>
{
private:
   const Material& m_material;
   unsigned int m_occurancesCount;

public:
   MaterialChecker(const Material& material)
      : m_material(material),
      m_occurancesCount(0)
   {}

   void visit(AbstractGraphicalNode& node)
   {
      if (m_material == node.getTechnique())
      {
         ++m_occurancesCount;
      }
   }

   unsigned int getOccurancesCount() const {return m_occurancesCount;}
};

///////////////////////////////////////////////////////////////////////////////
