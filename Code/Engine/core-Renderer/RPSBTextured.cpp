#include "core-Renderer/RPSBTextured.h"
#include "core-Renderer/Geometry.h"
#include "core-Renderer/StatefulRenderTreeBuilder.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSBTextured );
   PARENT( RPSceneBuilder );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

StateTreeNode* RPSBTextured::buildRenderTree( MemoryPool& pool, const Array< Geometry* >& visibleElems, RuntimeDataBuffer& data ) const
{
   StateTreeNode* root = StatefulRenderTreeBuilder::buildRenderTree( pool, visibleElems );
   return root;
}

///////////////////////////////////////////////////////////////////////////////
