#include "core-Renderer/MatrixSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPMatrixInput, RPNodeInput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

D3DXMATRIX RPMatrixInput::s_mtxIdentity;

///////////////////////////////////////////////////////////////////////////////

RPMatrixInput::RPMatrixInput( const std::string& name )
   : RPNodeInput( name )
{
   D3DXMatrixIdentity( &s_mtxIdentity );
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& RPMatrixInput::getValue( RuntimeDataBuffer& data ) const
{
   const RPNodeOutput* output = getOutput();
   if ( !output )
   {
      return s_mtxIdentity;
   }

   const RPMatrixOutput* typedOutput = dynamic_cast< const RPMatrixOutput* >( output );
   if ( typedOutput != NULL )
   {
      return typedOutput->getValue( data );
   }
   else
   {
      char tmp[256];
      sprintf_s( tmp, "Output '%s' input '%s' is connected to isn't a matrix output", output->getName().c_str(), getName().c_str() );
      throw std::runtime_error( tmp );
   }
}

///////////////////////////////////////////////////////////////////////////////

bool RPMatrixInput::canConnect( RPNodeOutput& output ) const
{
   RPMatrixOutput* typedOutput = dynamic_cast< RPMatrixOutput* >( &output );
   return typedOutput != NULL; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPMatrixOutput, RPNodeOutput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

D3DXMATRIX RPMatrixOutput::s_mtxIdentity;

///////////////////////////////////////////////////////////////////////////////

RPMatrixOutput::RPMatrixOutput( const std::string& name )
   : RPNodeOutput( name )
{
   D3DXMatrixIdentity( &s_mtxIdentity );
}

///////////////////////////////////////////////////////////////////////////////

void RPMatrixOutput::createLayout( RenderingPipelineMechanism& host ) const
{
   host.data().registerVar( m_value, s_mtxIdentity );
}

///////////////////////////////////////////////////////////////////////////////
