#ifndef _GRAPH_BLOCK_H
#error "This file can only be included from GraphBlock.h"
#else

#include "QPropertiesView.h"
#include "GraphBlockSocket.h"


///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
TGraphBlock< TNode, TBaseNode >::TGraphBlock()
   : m_node( NULL )
   , m_nodePtr( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
TGraphBlock< TNode, TBaseNode >::TGraphBlock( TNode& node )
   : m_node( NULL )
   , m_nodePtr( &node )
{

   // set the properties viewer as the central widget
   QPropertiesView* nodePropertiesRootView = new QPropertiesView();
   m_nodePtr->viewProperties( *nodePropertiesRootView );
   setCentralWidget( nodePropertiesRootView );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
TGraphBlock< TNode, TBaseNode >::~TGraphBlock()
{
   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
void TGraphBlock< TNode, TBaseNode >::initialize()
{
   m_node = new TResourceHandle< TNode >( *m_nodePtr );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
ReflectionObject* TGraphBlock< TNode, TBaseNode >::getNode() 
{ 
   return &m_node->get();
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
void TGraphBlock< TNode, TBaseNode >::onObjectLoaded()
{
   __super::onObjectLoaded();

   m_nodePtr = static_cast< TNode* >( getNode() );

   // set the properties viewer as the central widget
   QPropertiesView* nodePropertiesRootView = new QPropertiesView();
   m_nodePtr->viewProperties( *nodePropertiesRootView );
   setCentralWidget( nodePropertiesRootView );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BLOCK_H
