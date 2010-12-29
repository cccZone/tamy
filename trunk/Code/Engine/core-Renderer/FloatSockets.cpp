#include "core-Renderer/FloatSockets.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPFloatInput, RPNodeInput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPFloatInput::RPFloatInput( const std::string& name )
   : RPNodeInput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

float RPFloatInput::getValue()
{
   RPNodeOutput* output = getOutput();
   if ( !output )
   {
      return 0.0f;
   }

   RPFloatOutput* typedOutput = dynamic_cast< RPFloatOutput* >( output );
   if ( typedOutput != NULL )
   {
      return typedOutput->getValue();
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
   , m_value( 0.0f )
{
}

///////////////////////////////////////////////////////////////////////////////
