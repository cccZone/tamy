#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RenderingPipelineNode, ResourceObject );
   PROPERTY( InputsMap, m_inputs );
   PROPERTY( OutputsMap, m_outputs );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineNode::~RenderingPipelineNode()
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

void RenderingPipelineNode::initialize( RenderingPipelineMechanism& host )
{
   for( OutputsMap::iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->initialize( host );
   }

   onInitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::deinitialize( RenderingPipelineMechanism& host )
{
   onDeinitialize( host );

   for( OutputsMap::iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->deinitialize( host );
   }
}
   
///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::update( RenderingPipelineMechanism& host )
{
   onUpdate( host );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::getSubsequentNodes( std::vector< RenderingPipelineNode* >& outNodesToRun ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      const std::vector< RenderingPipelineNode* >& connectedNodes = (*it)->getConnectedNodes();
      outNodesToRun.insert( outNodesToRun.end(), connectedNodes.begin(), connectedNodes.end() );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RenderingPipelineNode::connectToInput( RPNodeOutput& output, const std::string& inputName )
{
   // looking at a connection from input's perspective, a node is interested in an output 
   // it originates at - it's from where we're gonna get our data
   RPNodeInput* input = findInput( inputName );
   if ( !input )
   {
      char tmp[128];
      sprintf_s( tmp, "Input '%s' not found", inputName.c_str() );
      throw std::runtime_error( tmp );
   }

   return input->connect( output );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::disconnectFromInput( const std::string& inputName )
{
   RPNodeInput* input = findInput( inputName );
   if ( !input )
   {
      char tmp[128];
      sprintf_s( tmp, "Input '%s' not found", inputName.c_str() );
      throw std::runtime_error( tmp );
   }

   input->disconnect();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::connectToOutput( RenderingPipelineNode& node, const std::string& outputName )
{
   // looking at a connection from input's perspective, a node is interested in a node 
   // the connection connects this node to - it's a node that's gonna get 
   // activated after this node's update is finished
   RPNodeOutput* output = findOutput( outputName );
   if ( !output )
   {
      char tmp[128];
      sprintf_s( tmp, "Output '%s' not found", outputName.c_str() );
      throw std::runtime_error( tmp );
   }

   output->connect( node );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::disconnectFromOutput( RenderingPipelineNode& node, const std::string& outputName )
{
   RPNodeOutput* output = findOutput( outputName );
   if ( !output )
   {
      char tmp[128];
      sprintf_s( tmp, "Output '%s' not found", outputName.c_str() );
      throw std::runtime_error( tmp );
   }

   output->disconnect( node );
}

///////////////////////////////////////////////////////////////////////////////

RPNodeInput* RenderingPipelineNode::findInput( const std::string& inputName ) const
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

RPNodeOutput* RenderingPipelineNode::findOutput( const std::string& outputName ) const
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

void RenderingPipelineNode::defineInput( RPNodeInput* input )
{
   RPNodeInput* existingInput = findInput( input->getName() );
   if ( existingInput == NULL )
   {
      m_inputs.push_back( input );
   }
   else
   {
      char tmp[128];
      sprintf_s( tmp, "Trying to override the definition of input '%s'", existingInput->getName().c_str() );

      delete input;

      throw std::runtime_error( tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::defineOutput( RPNodeOutput* output )
{
   RPNodeOutput* existingOutput = findOutput( output->getName() );
   if ( existingOutput == NULL )
   {
      m_outputs.push_back( output );
   }
   else
   {
      char tmp[128];
      sprintf_s( tmp, "Trying to override the definition of output '%s'", existingOutput->getName().c_str() );

      delete output;

      throw std::runtime_error( tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////
