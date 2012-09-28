#ifndef _GRAPH_BUILDER_NODE_H
#error "This file can only be included from GraphBuilderNode.h"
#else

#include "core/Assert.h"
#include "core/Log.h"
#include "core/GraphBuilderSockets.h"
#include "core/GraphBuilderNodeUtils.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
GraphBuilderNode< Impl >::~GraphBuilderNode()
{
   // remove inputs and outputs
   for( InputsMap::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it )
   {
      delete *it;
   }
   m_inputs.clear();

   for( OutputsMap::iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
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
      m_inputs.push_back( input );
      notify( GBNO_INPUTS_CHANGED );
   }
   else
   {
      char tmp[128];
      sprintf_s( tmp, "Trying to override the definition of input '%s'", existingInput->getName().c_str() );
      LOG( tmp );

      delete input;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::defineInputs( const std::vector< TInputSocket* >& inputs )
{
   uint inputsAdded = 0;

   uint count = inputs.size();
   for ( uint i = 0; i < count; ++i )
   {
      TInputSocket* newInput = inputs[i];

      TInputSocket* existingInput = findInput( newInput->getName() );
      if ( existingInput == NULL )
      {
         m_inputs.push_back( newInput );
         ++inputsAdded;
      }
      else
      {
         char tmp[128];
         sprintf_s( tmp, "Trying to override the definition of input '%s'", existingInput->getName().c_str() );
         LOG( tmp );

         delete newInput;
      }
   }

   if ( inputsAdded > 0 )
   {
      notify( GBNO_INPUTS_CHANGED );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::removeAllInputs()
{
   if ( m_inputs.empty() )
   {
      // there's nothing to be removed here
      return;
   }

   uint count = m_inputs.size();
   for ( uint i = 0; i < count; ++i )
   {
      TInputSocket* input = m_inputs[i];
      delete input;
   }
   m_inputs.clear();

   notify( GBNO_INPUTS_CHANGED );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::removeSelectedInputs( const std::vector< std::string >& inputNames )
{
   if ( m_inputs.empty() || inputNames.empty() )
   {
      // there's nothing to be removed here
      return;
   }

   uint namesCount = inputNames.size();
   for ( uint nameIdx = 0; nameIdx < namesCount; ++nameIdx )
   {
      const std::string& removedInputName = inputNames[nameIdx];

      uint count = m_inputs.size();
      for ( uint i = 0; i < count; ++i )
      {
         if ( m_inputs[i]->getName() == removedInputName )
         {
            TInputSocket* input = m_inputs[i];
            delete input;
            m_inputs.erase( m_inputs.begin() + i );
            break;
         }
      }
   }

   notify( GBNO_INPUTS_CHANGED );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::redefineInputs( const std::vector< TInputSocket* >& inputs )
{
   int numInputsChanged = GraphBuilderNodeUtils::mergeSocketsLists( inputs, m_inputs );

   // notify about the changes, if any were made
   if ( numInputsChanged > 0 )
   {
      notify( GBNO_INPUTS_CHANGED );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::defineOutput( TOutputSocket* output )
{

   // make sure we're not overriding the output
   TOutputSocket* existingOutput = findOutput( output->getName() );
   if ( existingOutput == NULL )
   {
      m_outputs.push_back( output );
      notify( GBNO_OUTPUTS_CHANGED );
   }
   else
   {
      char tmp[128];
      sprintf_s( tmp, "Trying to override the definition of output '%s'", existingOutput->getName().c_str() );
      LOG( tmp );

      delete output;
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::defineOutputs( const std::vector< TOutputSocket* >& outputs )
{
   uint outputsAdded = 0;

   uint count = outputs.size();
   for ( uint i = 0; i < count; ++i )
   {
      TOutputSocket* newOutput = outputs[i];

      // make sure we're not overriding the output
      TOutputSocket* existingOutput = findOutput( newOutput->getName() );
      if ( existingOutput == NULL )
      {
         m_outputs.push_back( newOutput );
         ++outputsAdded;
      }
      else
      {
         char tmp[128];
         sprintf_s( tmp, "Trying to override the definition of output '%s'", existingOutput->getName().c_str() );
         LOG( tmp );

         delete newOutput;
      }
   }

   if ( outputsAdded > 0 )
   {
      // notify the listeners only if we have something to notify of - meaning that any
      // of the outputs were actually added to the node 
      notify( GBNO_OUTPUTS_CHANGED );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::removeAllOutputs()
{
   if ( m_outputs.empty() )
   {
      // there's nothing to be removed here
      return;
   }

   uint count = m_outputs.size();
   for ( uint i = 0; i < count; ++i )
   {
      TOutputSocket* output = m_outputs[i];
      delete output;
   }
   m_outputs.clear();

   notify( GBNO_OUTPUTS_CHANGED );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::removeSelectedOutputs( const std::vector< std::string >& outputNames )
{
   if ( m_outputs.empty() || outputNames.empty() )
   {
      // there's nothing to be removed here
      return;
   }

   uint namesCount = outputNames.size();
   for ( uint nameIdx = 0; nameIdx < namesCount; ++nameIdx )
   {
      const std::string& removedOutputName = outputNames[nameIdx];

      uint count = m_outputs.size();
      for ( uint i = 0; i < count; ++i )
      {
         if ( m_outputs[i]->getName() == removedOutputName )
         {
            TOutputSocket* output = m_outputs[i];
            delete output;
            m_outputs.erase( m_outputs.begin() + i );
            break;
         }
      }
   }

   notify( GBNO_OUTPUTS_CHANGED );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::redefineOutputs( const std::vector< TOutputSocket* >& outputs )
{
   int numOutputsChanged = GraphBuilderNodeUtils::mergeSocketsLists( outputs, m_outputs );

   // notify about the changes, if any were made
   if ( numOutputsChanged > 0 )
   {
      notify( GBNO_OUTPUTS_CHANGED );
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

template< typename Impl >
void GraphBuilderNode< Impl >::notifyInputsChanged()
{
   notify( GBNO_INPUTS_CHANGED );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Impl >
void GraphBuilderNode< Impl >::notifyOutputsChanged()
{
   notify( GBNO_OUTPUTS_CHANGED );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _GRAPH_BUILDER_NODE_H
