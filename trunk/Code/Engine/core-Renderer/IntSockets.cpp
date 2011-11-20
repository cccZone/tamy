#include "core-Renderer/IntSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPIntInput, RPNodeInput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPIntInput::RPIntInput( const std::string& name )
   : RPNodeInput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

int RPIntInput::getValue( RuntimeDataBuffer& data ) const
{
   const RPNodeOutput* output = getOutput();
   if ( !output )
   {
      return 0;
   }

   const RPIntOutput* typedOutput = dynamic_cast< const RPIntOutput* >( output );
   if ( typedOutput != NULL )
   {
      return typedOutput->getValue( data );
   }
   else
   {
      char tmp[256];
      sprintf_s( tmp, "Output '%s' input '%s' is connected to isn't an int output", output->getName().c_str(), getName().c_str() );
      throw std::runtime_error( tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RPIntInput::canConnect( RPNodeOutput& output ) const
{
   RPIntOutput* typedOutput = dynamic_cast< RPIntOutput* >( &output );
   return typedOutput != NULL; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPIntOutput, RPNodeOutput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPIntOutput::RPIntOutput( const std::string& name )
   : RPNodeOutput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

void RPIntOutput::createLayout( RenderingPipelineMechanism& host ) const
{
   host.data().registerVar( m_value );
}

///////////////////////////////////////////////////////////////////////////////
