#pragma once

#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingTechnique;
class AbstractGraphicalNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * This visitor will replace the material in all AbstractGraphicalNodes
 * in the specified nodes hierarchy.
 */
class RenderingTechniqueReplacer : public NodeVisitor,
                                   public TNodesVisitor<AbstractGraphicalNode>
{
private:
   const RenderingTechnique& m_source;
   RenderingTechnique& m_target;

public:
   RenderingTechniqueReplacer(const RenderingTechnique& source, RenderingTechnique& target);

   void visit(AbstractGraphicalNode& node);
};

///////////////////////////////////////////////////////////////////////////////
