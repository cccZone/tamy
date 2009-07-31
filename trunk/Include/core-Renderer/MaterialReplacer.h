#pragma once

#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"


///////////////////////////////////////////////////////////////////////////////

class Material;
class AbstractGraphicalNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * This visitor will replace the material in all AbstractGraphicalNodes
 * in the specified nodes hierarchy.
 */
class MaterialReplacer : public NodeVisitor,
                                   public TNodesVisitor<AbstractGraphicalNode>
{
private:
   const Material& m_source;
   Material& m_target;

public:
   MaterialReplacer(const Material& source, Material& target);

   void visit(AbstractGraphicalNode& node);
};

///////////////////////////////////////////////////////////////////////////////
