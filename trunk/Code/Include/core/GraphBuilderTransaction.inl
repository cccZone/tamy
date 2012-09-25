#ifndef _GRAPH_BUILDER_TRANSACTION_H
#error "This file can only be included from GraphBuilderTransaction.h"
#else


///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
GraphBuilderTransaction< Impl, NodeType >::GraphBuilderTransaction()
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
GraphBuilderTransaction< Impl, NodeType >::~GraphBuilderTransaction()
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::commit( GraphBuilder< Impl, NodeType >& graph )
{
   // inform listeners that the graph's about to change
   graph.notifyPreChange();

   // remove obsolete nodes
   uint count = m_nodesToRemove.size();
   for ( uint i = 0; i < count; ++i )
   {
      graph.removeNode( *m_nodesToRemove[i] );
   }

   // add new nodes
   count = m_nodesToAdd.size();
   for ( uint i = 0; i < count; ++i )
   {
      graph.addNode( m_nodesToAdd[i] );
   }

   // commit implementation specific data
   onCommit( static_cast< Impl& >( graph ) );

   // inform listeners that the graph's changed
   graph.notifyPostChange();
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::addNode( NodeType* node )
{
   putNodeForTransaction( node, m_nodesToAdd );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::removeNode( NodeType& node )
{
   putNodeForTransaction( &node, m_nodesToRemove );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl, typename NodeType >
void GraphBuilderTransaction< Impl, NodeType >::putNodeForTransaction( NodeType* node, std::vector< NodeType* >& collection ) const
{
   // look for duplicates
   uint count = collection.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( collection[i] == node )
      {
         // this one's already on the list
         return;
      }
   }

   collection.push_back( node );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_TRANSACTION_H