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

ShaderTexture* RPTextureInput::getTexture()
{
   RPNodeOutput* output = getOutput();
   if ( !output )
   {
      return NULL;
   }

   RPTextureOutput* typedOutput = dynamic_cast< RPTextureOutput* >( output );
   if ( typedOutput == NULL )
   {
      char tmp[256];
      sprintf_s( tmp, "Output '%s' input '%s' is connected to isn't a texture output", output->getName().c_str(), getName().c_str() );
      throw std::runtime_error( tmp );
   }

   return typedOutput->getTexture();
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
   , m_renderTarget( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void RPTextureOutput::initialize( RenderingPipelineMechanism& host )
{
   if ( m_useBackBuffer )
   {
      m_renderTarget = NULL;
   }
   else
   {
      m_renderTarget = &host.getRenderTarget( m_renderTargetId );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPTextureOutput::deinitialize( RenderingPipelineMechanism& host )
{
   m_renderTarget = NULL;
}

///////////////////////////////////////////////////////////////////////////////

ShaderTexture* RPTextureOutput::getTexture() const
{
   if ( m_useBackBuffer )
   {
      // TODO: !!!!!!!! return m_renderer->getBackBufferTexture();
      return NULL;
   }
   else
   {
      return m_renderTarget;
   }
}

///////////////////////////////////////////////////////////////////////////////
