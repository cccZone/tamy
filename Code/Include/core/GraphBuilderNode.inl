#ifndef _GRAPH_BUILDER_NODE_H
#error "This file can only be included from GraphBuilderNode.h"
#else

#include "core/Assert.h"
#include "core/GraphBuilderSockets.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
GraphBuilderNode< Impl >::~GraphBuilderNode()
{
   // remove inputs and outputs
   for( InputsMap::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it )
   {
      (*it)->detachObserver( *this );
      delete *it;
   }
   m_inputs.clear();

   for( OutputsMap::iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->detachObserver( *this );
      delete *it;
   }
   m_outputs.clear();
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::getSubsequentNodes( std::vector< Impl* >& outNodesToRun ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      const std::vector< Impl* >& connectedNodes = (*it)->getConnectedNodes();
      outNodesToRun.insert( outNodesToRun.end(), connectedNodes.begin(), connectedNodes.end() );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
bool GraphBuilderNode< Impl >::isConnectedWith( Impl* otherNode ) const
{
   uint outputsCount = m_outputs.size();
   for( uint outputIdx = 0; outputIdx < outputsCount; ++outputIdx )
   {
      const std::vector< Impl* >& connectedNodes = m_outputs[outputIdx]->getConnectedNodes();

      uint nodesCount = connectedNodes.size();
      for ( uint nodeIdx = 0; nodeIdx < nodesCount; ++nodeIdx )
      {
         if ( connectedNodes[nodeIdx] == otherNode )
         {
            // found it - there is a connection between the nodes.
            return true;
         }
      }
   }

   // the two nodes are not connected
   return false;
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
bool GraphBuilderNode< Impl >::connect( const std::string& outputName, Impl& destNode, const std::string& inputName )
{
   // looking at a connection from input's perspective, a node is interested in a node 
   // the connection connects this node to - it's a node that's gonna get 
   // activated after this node's update is finished
   TOutputSocket* output = findOutput( outputName );
   if ( !output )
   {
      char tmp[128];
      sprintf_s( tmp, "Output '%s' not found", outputName.c_str() );
      ASSERT_MSG( false, tmp );
   }

   output->connect( destNode );
   notify( GBNO_CHANGED );

   // looking at a connection from input's perspective, a node is interested in an output 
   // it originates at - it's from where we're gonna get our data
   TInputSocket* input = destNode.findInput( inputName );
   if ( !input )
   {
      char tmp[128];
      sprintf_s( tmp, "Input '%s' not found", inputName.c_str() );
      ASSERT_MSG( false, tmp );
   }

   bool result = input->connect( *output );
   if ( result )
   {
      destNode.notify( GBNO_CHANGED );
   }

   return result;
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::disconnect( Impl& destNode, const std::string& inputName )
{
   // disconnect the other node's input
   TInputSocket* input = destNode.findInput( inputName );
   if ( !input )
   {
      // input could have been removed - and since there's nothing to do here, bail
      return;
   }

   input->disconnect();
   destNode.notify( GBNO_CHANGED );

   // disconnect the output
   TOutputSocket* output = input->getOutput();
   if ( output )
   {
      output->disconnect( destNode );
      notify( GBNO_CHANGED );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::disconnect( Impl& destNode )
{
   uint outputsCount = m_outputs.size();
   for ( uint i = 0; i < outputsCount; ++i )
   {
      TOutputSocket* output = m_outputs[i];
      if ( output )
      {
         output->disconnect( destNode );
      }
   }

   // send notifications
   destNode.notify( GBNO_CHANGED );
   notify( GBNO_CHANGED );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
GBNodeInput< Impl >* GraphBuilderNode< Impl >::findInput( const std::string& inputName ) const
{
   for ( InputsMap::const_iterator it = m_inputs.begin(); it != m_inputs.end(); ++it )
   {
      if ( (*it)->getName() == inputName )
      {
         return *it;
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
GBNodeOutput< Impl >* GraphBuilderNode< Impl >::findOutput( const std::string& outputName ) const
{
   for ( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      if ( (*it)->getName() == outputName )
      {
         return *it;
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::defineInput( TInputSocket* input )
{
   TInputSocket* existingInput = findInput( input->getName() );
   if ( existingInput == NULL )
   {
      input->attachObserver( *this );
      m_inputs.push_back( input );

      notify( GBNO_INPUTS_CHANGED );
   }
   else
   {
      char tmp[128];
      sprintf_s( tmp, "Trying to override the definition of input '%s'", existingInput->getName().c_str() );

      delete input;
      ASSERT_MSG( false, tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::removeInput( const std::string& name )
{
   for ( InputsMap::const_iterator it = m_inputs.begin(); it != m_inputs.end(); ++it )
   {
      if ( (*it)->getName() == name )
      {
         (*it)->detachObserver( *this );

         delete *it;
         m_inputs.erase( it );

         notify( GBNO_INPUTS_CHANGED );

         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::defineOutput( TOutputSocket* output )
{
   TOutputSocket* existingOutput = findOutput( output->getName() );
   if ( existingOutput == NULL )
   {
      output->attachObserver( *this );
      m_outputs.push_back( output );

      notify( GBNO_OUTPUTS_CHANGED );
   }
   else
   {
      char tmp[128];
      sprintf_s( tmp, "Trying to override the definition of output '%s'", existingOutput->getName().c_str() );

      delete output;
      ASSERT_MSG( false, tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::removeOutput( const std::string& name )
{
   for ( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      if ( (*it)->getName() == name )
      {
         (*it)->detachObserver( *this );

         delete *it;
         m_outputs.erase( it );

         notify( GBNO_OUTPUTS_CHANGED );

         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::onBulkSocketsInitialization()
{
   // attach self as an observer of the sockets
   for( InputsMap::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it )
   {
      (*it)->attachObserver( *this );
   }
   for( OutputsMap::iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->attachObserver( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::update( GBNodeSocket& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::update( GBNodeSocket& subject, const GBNodeSocketOperation& msg )
{
   if ( msg == GBNSO_CHANGED )
   {
      notify( GBNO_CHANGED );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl > template< typename T >
const T& GraphBuilderNode< Impl >::getInput( const std::string& inputName ) const
{
   TInputSocket* input = findInput( inputName );
   if ( input == NULL)
   {
      char tmp[128];
      sprintf_s( tmp, "Unable to find input '%s'", inputName.c_str() );
      ASSERT_MSG( false, tmp );
   }

   T* dataSource = DynamicCast< T >( input );
   if ( dataSource == NULL )
   {
      char tmp[128];
      sprintf_s( tmp, "Input's '%s' type doesn't match the specified one", inputName.c_str() );
      ASSERT_MSG( false, tmp );
   }

   return *dataSource;
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl > template< typename T >
const T& GraphBuilderNode< Impl >::getOutput( const std::string& outputName ) const
{
   TOutputSocket* output = findOutput( outputName );
   if ( output == NULL )
   {
      char tmp[128];
      sprintf_s( tmp, "Unable to find output '%s'", outputName.c_str() );
      ASSERT_MSG( false, tmp );
   }

   T* castOutput = DynamicCast< T >( output );
   if ( castOutput == NULL )
   {
      char tmp[128];
      sprintf_s( tmp, "Output's '%s' type doesn't match the specified one", outputName.c_str() );
      ASSERT_MSG( false, tmp );
   }

   return *castOutput;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_NODE_H
