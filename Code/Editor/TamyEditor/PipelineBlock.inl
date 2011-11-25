#ifndef _PIPELINE_BLOCK_H
#error "This file can only be included from PipelineBlock.h"
#else


///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
TPipelineBlock< TNode, TBaseNode >::TPipelineBlock()
   : m_node( NULL )
   , m_nodePtr( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
TPipelineBlock< TNode, TBaseNode >::TPipelineBlock( TNode& node )
   : m_node( NULL )
   , m_nodePtr( &node )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
TPipelineBlock< TNode, TBaseNode >::~TPipelineBlock()
{
   TNode& nodePtr = dynamic_cast< TNode& >( getNode() );
   nodePtr.detachObserver( *this );

   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
void TPipelineBlock< TNode, TBaseNode >::initialize()
{
   m_node = new TResourceHandle< TNode >( *m_nodePtr );
   m_nodePtr->attachObserver( *this );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
Object& TPipelineBlock< TNode, TBaseNode >::getNode() 
{ 
   return m_node->get();
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
void TPipelineBlock< TNode, TBaseNode >::onObjectLoaded()
{
   __super::onObjectLoaded();

   TNode& nodePtr = dynamic_cast< TNode& >( getNode() );
   nodePtr.attachObserver( *this );
}

///////////////////////////////////////////////////////////////////////////////

template<  typename TNode, typename TBaseNode >
void TPipelineBlock< TNode, TBaseNode >::update( TBaseNode& node )
{
   refreshInputs( node );
   refreshOutputs( node );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
void TPipelineBlock< TNode, TBaseNode >::update( TBaseNode& node, const GraphBuilderNodeOperation& msg )
{
   if ( msg == GBNO_INPUTS_CHANGED )
   {
      refreshInputs( node );
   }
   else if ( msg == GBNO_OUTPUTS_CHANGED )
   {
      refreshOutputs( node );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
void TPipelineBlock< TNode, TBaseNode >::refreshInputs( TBaseNode& node )
{
   // first - gather the names of all inputs the block currently has
   std::set< std::string > inputNames;
   getAllSockets( GBSP_INPUT, inputNames );

   // now go through the node's inputs and see which ones were just created
   const std::vector< GBNodeInput< TBaseNode >* >& inputs = node.getInputs();
   for ( std::vector< GBNodeInput< TBaseNode >* >::const_iterator it = inputs.begin(); it != inputs.end(); ++it )
   {
      // check if an input with this name doesn't already exist - if it does, don't create one
      const std::string& inputName = (*it)->getName();
      if ( inputNames.find( inputName ) == inputNames.end() )
      {
         GraphBlockSocket* socket = m_socketsFactory->create( **it );
         socket->initialize( this, GBSP_INPUT, inputName.c_str() );
         addSocket( socket );
      }

      inputNames.erase( inputName );
   }

   // now go through the remaining input names - those are the inputs that were deleted - and remove them
   removeSockets( GBSP_INPUT, inputNames );
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode, typename TBaseNode >
void TPipelineBlock< TNode, TBaseNode >::refreshOutputs( TBaseNode& node )
{
   // first - gather the names of all outputs the block currently has
   std::set< std::string > outputNames;
   getAllSockets( GBSP_OUTPUT, outputNames );

   // now go through the node's outputs and see which ones were just created
   const std::vector< GBNodeOutput< TBaseNode >* >& outputs = node.getOutputs();
   for ( std::vector< GBNodeOutput< TBaseNode >* >::const_iterator it = outputs.begin(); it != outputs.end(); ++it )
   {
      // check if an input with this name doesn't already exist - if it does, don't create one
      const std::string& outputName = (*it)->getName();
      if ( outputNames.find( outputName ) == outputNames.end() )
      {
         GraphBlockSocket* socket = m_socketsFactory->create( **it );
         socket->initialize( this, GBSP_OUTPUT, outputName.c_str() );
         addSocket( socket );
      }

      outputNames.erase( outputName );
   }

   // now go through the remaining output names - those are the inputs that were deleted - and remove them
   removeSockets( GBSP_OUTPUT, outputNames );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PIPELINE_BLOCK_H
