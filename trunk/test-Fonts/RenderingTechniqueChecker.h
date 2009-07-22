#pragma once

#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "core-Renderer\AbstractGraphicalNode.h"
#include "core-Renderer\RenderingTechnique.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingTechniqueChecker : public NodeVisitor,
                                  public TNodesVisitor<AbstractGraphicalNode>
{
private:
   const RenderingTechnique& m_technique;
   unsigned int m_occurancesCount;

public:
   MaterialChecker(const RenderingTechnique& technique)
      : m_technique(technique),
      m_occurancesCount(0)
   {}

   void visit(AbstractGraphicalNode& node)
   {
      if (m_technique == node.getTechnique())
      {
         ++m_occurancesCount;
      }
   }

   unsigned int getOccurancesCount() const {return m_occurancesCount;}
};

///////////////////////////////////////////////////////////////////////////////
