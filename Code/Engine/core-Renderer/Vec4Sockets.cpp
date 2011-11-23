#include "core-Renderer/Vec4Sockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVec4Input, RPNodeInput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPVec4Input::RPVec4Input( const std::string& name )
   : RPNodeInput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

const D3DXVECTOR4& RPVec4Input::getValue( RuntimeDataBuffer& data ) const
{
   const RPNodeOutput* output = getOutput();
   if ( !output )
   {
      static D3DXVECTOR4 emptyVec( 0, 0, 0, 0 );
      return emptyVec;
   }

   const RPVec4Output* typedOutput = dynamic_cast< const RPVec4Output* >( output );
   if ( typedOutput != NULL )
   {
      return typedOutput->getValue( data );
   }
   else
   {
      char tmp[256];
      sprintf_s( tmp, "Output '%s' input '%s' is connected to isn't a vector output", output->getName().c_str(), getName().c_str() );
      throw std::runtime_error( tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RPVec4Input::canConnect( RPNodeOutput& output ) const
{
   RPVec4Output* typedOutput = DynamicCast< RPVec4Output >( &output );
   return typedOutput != NULL; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPVec4Output, RPNodeOutput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPVec4Output::RPVec4Output( const std::string& name )
   : RPNodeOutput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

void RPVec4Output::createLayout( RenderingPipelineMechanism& host ) const
{
   host.data().registerVar( m_value, D3DXVECTOR4( 0, 0, 0, 0 ) );
}

///////////////////////////////////////////////////////////////////////////////
