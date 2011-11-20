#include "core-Renderer/BoolSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBoolInput, RPNodeInput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBoolInput::RPBoolInput( const std::string& name )
   : RPNodeInput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

bool RPBoolInput::getValue( RuntimeDataBuffer& data ) const
{
   const RPNodeOutput* output = getOutput();
   if ( !output )
   {
      return false;
   }

   const RPBoolOutput* typedOutput = dynamic_cast< const RPBoolOutput* >( output );
   if ( typedOutput != NULL )
   {
      return typedOutput->getValue( data );
   }
   else
   {
      char tmp[256];
      sprintf_s( tmp, "Output '%s' input '%s' is connected to isn't a bool output", output->getName().c_str(), getName().c_str() );
      throw std::runtime_error( tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RPBoolInput::canConnect( RPNodeOutput& output ) const
{
   RPBoolOutput* typedOutput = dynamic_cast< RPBoolOutput* >( &output );
   return typedOutput != NULL; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBoolOutput, RPNodeOutput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBoolOutput::RPBoolOutput( const std::string& name )
   : RPNodeOutput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

void RPBoolOutput::createLayout( RenderingPipelineMechanism& host ) const
{
   host.data().registerVar( m_value );
}

///////////////////////////////////////////////////////////////////////////////
