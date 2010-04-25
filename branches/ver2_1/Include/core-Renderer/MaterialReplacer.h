#pragma once

#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"


///////////////////////////////////////////////////////////////////////////////

class Material;
class RenderableNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * This visitor will replace the material in all RenderableNode
 * in the specified nodes hierarchy.
 */
class MaterialReplacer : public NodeVisitor,
                         public TNodesVisitor<RenderableNode>
{
private:
   const Material& m_source;
   Material& m_target;

public:
   MaterialReplacer(const Material& source, Material& target);

   void visit(RenderableNode& node);
};

///////////////////////////////////////////////////////////////////////////////
