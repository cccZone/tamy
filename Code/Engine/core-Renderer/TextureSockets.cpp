#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPTextureInput, RPNodeInput );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPTextureInput::RPTextureInput( const std::string& name )
   : RPNodeInput( name )
{
}

///////////////////////////////////////////////////////////////////////////////

ShaderTexture* RPTextureInput::getTexture( RuntimeDataBuffer& data ) const
{
   const RPNodeOutput* output = getOutput();
   if ( !output )
   {
      return NULL;
   }

   const RPTextureOutput* typedOutput = dynamic_cast< const RPTextureOutput* >( output );
   if ( typedOutput == NULL )
   {
      char tmp[256];
      sprintf_s( tmp, "Output '%s' input '%s' is connected to isn't a texture output", output->getName().c_str(), getName().c_str() );
      throw std::runtime_error( tmp );
   }

   return typedOutput->getTexture( data );
}

///////////////////////////////////////////////////////////////////////////////

bool RPTextureInput::canConnect( RPNodeOutput& output ) const
{
   RPTextureOutput* typedOutput = dynamic_cast< RPTextureOutput* >( &output );
   return typedOutput != NULL; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPTextureOutput, RPNodeOutput );
   PROPERTY_EDIT( "Use back buffer", bool, m_useBackBuffer );
   PROPERTY_EDIT( "Render target id", std::string, m_renderTargetId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPTextureOutput::RPTextureOutput( const std::string& name )
   : RPNodeOutput( name )
   , m_useBackBuffer( true )
{
}

///////////////////////////////////////////////////////////////////////////////

void RPTextureOutput::createLayout( RenderingPipelineMechanism& host ) const
{
   host.data().registerVar( m_renderTarget );
}

///////////////////////////////////////////////////////////////////////////////

void RPTextureOutput::initialize( RenderingPipelineMechanism& host ) const
{
   if ( m_useBackBuffer )
   {
      host.data()[ m_renderTarget ] = NULL;
   }
   else
   {
      host.data()[ m_renderTarget ] = &host.getRenderTarget( m_renderTargetId );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPTextureOutput::deinitialize( RenderingPipelineMechanism& host ) const
{
   host.data()[ m_renderTarget ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget* RPTextureOutput::getRenderTarget( RuntimeDataBuffer& data ) const 
{ 
   return data[ m_renderTarget ]; 
}

///////////////////////////////////////////////////////////////////////////////

void RPTextureOutput::setRenderTargetID( const std::string renderTargetId ) 
{ 
   m_renderTargetId = renderTargetId; 
   m_useBackBuffer = m_renderTargetId.empty();
   notify( RPNSO_CHANGED );
}

///////////////////////////////////////////////////////////////////////////////

ShaderTexture* RPTextureOutput::getTexture( RuntimeDataBuffer& data ) const
{
   if ( m_useBackBuffer )
   {
      return NULL;
   }
   else
   {
      return data[ m_renderTarget ];
   }
}

///////////////////////////////////////////////////////////////////////////////
