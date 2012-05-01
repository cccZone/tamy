#include "core-Renderer/RPSBTextured.h"
#include "core-Renderer/Geometry.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSBTextured );
   PARENT( RPSceneBuilder );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

StateTreeNode* RPSBTextured::buildRenderTree( MemoryPool& pool, const Array< Geometry* >& visibleElems, RuntimeDataBuffer& data ) const
{
   StateTreeNode* root = new ( pool ) StateTreeNode();

   unsigned int elemsCount = visibleElems.size();
   for ( unsigned int i = 0; i < elemsCount; ++i )
   {     
      Geometry* geometry = visibleElems[i];
      const RenderStatesVec& states = geometry->getRenderStates();
      // at this point the states are already sorted

      // states span into the tree depth - so each branch is a unique state,
      // and its leaves contain geometry to render

      // find the child node for the state of the added element
      StateTreeNode* currNode = root;
      unsigned int statesCount = states.size();

      for( unsigned int j = 0; j < statesCount; ++j )
      {
         RenderState* checkedState = states[j];

         // get the child of the current node, and if there's none - create one
         if ( currNode->m_child == NULL )
         {
            currNode->m_child = new ( pool ) StateTreeNode();
         }
         currNode = currNode->m_child;


         // check the siblings and look for a similar state
         StateTreeNode* prevNode = currNode;
         while ( currNode && currNode->compareState( checkedState ) == false )
         {
            prevNode = currNode;
            currNode = currNode->m_sibling;
         }

         if ( currNode == NULL )
         {
            // there is no node with a matching state - create a new one
            prevNode->m_sibling = new ( pool ) StateTreeNode( *checkedState );
            currNode = prevNode->m_sibling;
         }
      }

      // there are no more states to check - add the geometry
      if ( statesCount > 0 )
      {
         // but don't render the geometry using some random state, if it doesn't have one of its own
         new ( pool ) GeometryNode( currNode->m_geometryNode, *geometry );
      }
   }

   return root;
}

///////////////////////////////////////////////////////////////////////////////
