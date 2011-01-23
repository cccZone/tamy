#include "core-Renderer/FloatSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPFloatInput, RPNodeInput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPFloatInput::RPFloatInput( const std::string& name )
   : RPNodeInput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

float RPFloatInput::getValue( RuntimeDataBuffer& data ) const
{
   const RPNodeOutput* output = getOutput();
   if ( !output )
   {
      return 0.0f;
   }

   const RPFloatOutput* typedOutput = dynamic_cast< const RPFloatOutput* >( output );
   if ( typedOutput != NULL )
   {
      return typedOutput->getValue( data );
   }
   else
   {
      char tmp[256];
      sprintf_s( tmp, "Output '%s' input '%s' is connected to isn't a float output", output->getName().c_str(), getName().c_str() );
      throw std::runtime_error( tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RPFloatInput::canConnect( RPNodeOutput& output ) const
{
   RPFloatOutput* typedOutput = dynamic_cast< RPFloatOutput* >( &output );
   return typedOutput != NULL; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPFloatOutput, RPNodeOutput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPFloatOutput::RPFloatOutput( const std::string& name )
   : RPNodeOutput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

void RPFloatOutput::createLayout( RenderingPipelineMechanism& host ) const
{
   host.data().registerVar( m_value );
}

///////////////////////////////////////////////////////////////////////////////
