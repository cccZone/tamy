#include "TabsTree.h"


///////////////////////////////////////////////////////////////////////////////

void TreeTabNode::executeBFS( TreeTabNode* rootNode, TabsTreeOperation& operation )
{
   std::list< TreeTabNode* > nodes;
   nodes.push_back( rootNode );

   while( !nodes.empty() )
   {
      TreeTabNode* analyzedNode = nodes.front();
      nodes.pop_front();

      LeafTabWidget* leafWidget = analyzedNode->asLeaf();
      if ( leafWidget )
      {
         operation( leafWidget );
      }

      analyzedNode->collectChildren( nodes );
   }
}

///////////////////////////////////////////////////////////////////////////////
