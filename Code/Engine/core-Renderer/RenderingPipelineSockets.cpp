#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RPNodeSocket, Object );
   PROPERTY( std::string, m_name );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPNodeSocket::RPNodeSocket( const std::string& name )
   : m_name( name )
{
}

///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RPNodeInput, RPNodeSocket );
   PROPERTY( RPNodeOutput*, m_connectedOutput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPNodeInput::RPNodeInput( const std::string& name )
   : RPNodeSocket( name )
   , m_connectedOutput( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

RPNodeInput::~RPNodeInput()
{
   m_connectedOutput = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool RPNodeInput::connect( RPNodeOutput& output )
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

void RPNodeInput::disconnect()
{
   m_connectedOutput = NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT( RPNodeOutput, RPNodeSocket );
   PROPERTY( std::vector< RenderingPipelineNode* >, m_connectedNodes );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPNodeOutput::RPNodeOutput( const std::string& name )
   : RPNodeSocket( name )
{
}

///////////////////////////////////////////////////////////////////////////////

RPNodeOutput::~RPNodeOutput()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPNodeOutput::connect( RenderingPipelineNode& node )
{
   // we don't want duplicates - add the node only if it's not already on the list
   std::vector< RenderingPipelineNode* >::iterator it = std::find( m_connectedNodes.begin(), m_connectedNodes.end(), &node );
   if ( it == m_connectedNodes.end() )
   {
      m_connectedNodes.push_back( &node );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPNodeOutput::disconnect( RenderingPipelineNode& node )
{
   std::vector< RenderingPipelineNode* >::iterator it = std::find( m_connectedNodes.begin(), m_connectedNodes.end(), &node );
   if ( it != m_connectedNodes.end() )
   {
      m_connectedNodes.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////
