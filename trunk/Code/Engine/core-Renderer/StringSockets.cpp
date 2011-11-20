#include "core-Renderer/StringSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPStringInput, RPNodeInput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPStringInput::RPStringInput( const std::string& name )
   : RPNodeInput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

const std::string& RPStringInput::getValue( RuntimeDataBuffer& data ) const
{
   const RPNodeOutput* output = getOutput();
   if ( !output )
   {
      static std::string emptyStr( "" );
      return emptyStr;
   }

   const RPStringOutput* typedOutput = dynamic_cast< const RPStringOutput* >( output );
   if ( typedOutput != NULL )
   {
      return typedOutput->getValue( data );
   }
   else
   {
      char tmp[256];
      sprintf_s( tmp, "Output '%s' input '%s' is connected to isn't a string output", output->getName().c_str(), getName().c_str() );
      throw std::runtime_error( tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RPStringInput::canConnect( RPNodeOutput& output ) const
{
   RPStringOutput* typedOutput = dynamic_cast< RPStringOutput* >( &output );
   return typedOutput != NULL; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPStringOutput, RPNodeOutput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPStringOutput::RPStringOutput( const std::string& name )
   : RPNodeOutput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

void RPStringOutput::createLayout( RenderingPipelineMechanism& host ) const
{
   host.data().registerVar( m_value );
}

///////////////////////////////////////////////////////////////////////////////
