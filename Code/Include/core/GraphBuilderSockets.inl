#ifndef _GRAPH_BUILDER_SOCKETS_H
#error "This file can only be included from GraphBuilderSockets.h"
#else


///////////////////////////////////////////////////////////////////////////////

template< typename NodeType >
GBNodeOutput< NodeType >::GBNodeOutput( const std::string& name )
   : GBNodeSocket( name )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename NodeType >
GBNodeOutput< NodeType >::~GBNodeOutput()
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename NodeType >
void GBNodeOutput< NodeType >::connect( NodeType& node )
{
   // we don't want duplicates - add the node only if it's not already on the list
   std::vector< NodeType* >::iterator it = std::find( m_connectedNodes.begin(), m_connectedNodes.end(), &node );
   if ( it == m_connectedNodes.end() )
   {
      m_connectedNodes.push_back( &node );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename NodeType >
void GBNodeOutput< NodeType >::disconnect( NodeType& node )
{
   std::vector< NodeType* >::iterator it = std::find( m_connectedNodes.begin(), m_connectedNodes.end(), &node );
   if ( it != m_connectedNodes.end() )
   {
      m_connectedNodes.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TOutputSocket >
GBNodeInput< TOutputSocket >::GBNodeInput( const std::string& name )
   : GBNodeSocket( name )
   , m_connectedOutput( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TOutputSocket >
GBNodeInput< TOutputSocket >::~GBNodeInput()
{
   m_connectedOutput = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TOutputSocket >
bool GBNodeInput< TOutputSocket >::connect( TOutputSocket& output )
{
   if ( m_connectedOutput != NULL || canConnect( output ) == false )
   {
      return false;
   }
   else
   {
      m_connectedOutput = &output;
      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename TOutputSocket >
void GBNodeInput< TOutputSocket >::disconnect()
{
   m_connectedOutput = NULL;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_SOCKETS_H
