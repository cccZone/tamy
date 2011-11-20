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

void RenderingPipelineNode::createLayout( RenderingPipelineMechanism& host ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->createLayout( host );
   }

   onCreateLayout( host );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::initialize( RenderingPipelineMechanism& host ) const
{
   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->initialize( host );
   }

   onInitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::deinitialize( RenderingPipelineMechanism& host ) const
{
   onDeinitialize( host );

   for( OutputsMap::const_iterator it = m_outputs.begin(); it != m_outputs.end(); ++it )
   {
      (*it)->deinitialize( host );
   }
}
   
///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::update( RenderingPipelineMechanism& host ) const
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

   bool result = input->connect( output );
   if ( result )
   {
      notify( RPNO_CHANGED );
   }

   return result;
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
   notify( RPNO_CHANGED );
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
   notify( RPNO_CHANGED );
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
   notify( RPNO_CHANGED );
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
      input->attachObserver( *this );
      m_inputs.push_back( input );

      notify( RPNO_INPUTS_CHANGED );
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

void RenderingPipelineNode::removeInput( const std::string& name )
{
   for ( InputsMap::const_iterator it = m_inputs.begin(); it != m_inputs.end(); ++it )
   {
      if ( (*it)->getName() == name )
      {
         (*it)->detachObserver( *this );

         delete *it;
         m_inputs.erase( it );

         notify( RPNO_INPUTS_CHANGED );

         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::defineOutput( RPNodeOutput* output )
{
   RPNodeOutput* existingOutput = findOutput( output->getName() );
   if ( existingOutput == NULL )
   {
      output->attachObserver( *this );
      m_outputs.push_back( output );

      notify( RPNO_OUTPUTS_CHANGED );
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

void RenderingPipelineNode::onObjectLoaded()
{
   __super::onObjectLoaded();

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

void RenderingPipelineNode::onPropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_inputs" )
   {
      notify( RPNO_INPUTS_CHANGED );
   }
   else if ( property.getName() == "m_outputs" )
   {
      notify( RPNO_OUTPUTS_CHANGED );
   }
   else
   {
      notify( RPNO_CHANGED );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::update( RPNodeSocket& subject )
{
   // do nothing - this is an initial update
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineNode::update( RPNodeSocket& subject, const RPNodeSocketOperation& msg )
{
   if ( msg == RPNSO_CHANGED )
   {
      notify( RPNO_CHANGED );
   }
}

///////////////////////////////////////////////////////////////////////////////
