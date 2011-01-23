#include "core-Renderer/RPDownsampleNode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPDownsampleNode, RPPostProcessNode )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPDownsampleNode::RPDownsampleNode()
{
   defineInput( new RPTextureInput ( "InputTex" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPDownsampleNode::~RPDownsampleNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPDownsampleNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   __super::onCreateLayout( host );

   RuntimeDataBuffer& data = host.data();

   data.registerVar( m_downsamplePass );
   data.registerVar( m_inputTex );
   data.registerVar( m_downsampleTarget );
   data.registerVar( m_renderer );
}

///////////////////////////////////////////////////////////////////////////////

void RPDownsampleNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   __super::onInitialize( host );

   RuntimeDataBuffer& data = host.data();

   // acquire the input texture
   data[ m_inputTex ] = getInput< RPTextureInput >( "InputTex" ).getTexture( data );
   data[ m_downsampleTarget ] = getOutput< RPTextureOutput >( "Output" ).getRenderTarget( data );

   // initialize the pixel shader
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   data[ m_downsamplePass ] = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_DownSample4x4.psh" );
   data[ m_downsamplePass ]->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "DownSample4x4" );

   data[ m_renderer ] = &host.getRenderer();
   data[ m_renderer ]->implement< PixelShader >( *data[ m_downsamplePass ] );
}

///////////////////////////////////////////////////////////////////////////////

void RPDownsampleNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   delete data[ m_downsamplePass ];
   data[ m_downsamplePass ] = NULL;

   data[ m_inputTex ] = NULL;
   data[ m_downsampleTarget ] = NULL;
   data[ m_renderer ] = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPDownsampleNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   Renderer* renderer = data[ m_renderer ];
   PixelShader* downsamplePass = data[ m_downsamplePass ];
   ShaderTexture* inputTex = data[ m_inputTex ];
   RenderTarget* downsampleTarget = data[ m_downsampleTarget ];

   if ( !inputTex )
   {
      return;
   }

   downsamplePass->setTexture( "inputTex", *inputTex );

   // We need to compute the sampling offsets used for this pass.
   // A 4x4 sampling pattern is used, so we need to generate 16 offsets

   // Compute the offsets required for down-sampling. If constant-storage space
   // is important then this code could be packed into 8xFloat4's. The code here
   // is intentionally less efficient to aid readability...
   D3DXVECTOR4 dsOffsets[16];
   int idx = 0;
   for( int i = -2; i < 2; i++ )
   {
      for( int j = -2; j < 2; j++ )
      {
         dsOffsets[idx++] = D3DXVECTOR4(
            ( static_cast< float >( i ) + 0.5f ) * ( 1.0f / static_cast< float >( downsampleTarget->getWidth() ) ),
            ( static_cast< float >( j ) + 0.5f ) * ( 1.0f / static_cast< float >( downsampleTarget->getHeight() ) ),
            0.0f, // unused 
            0.0f  // unused
            );
      }
   }

   downsamplePass->setVec4Array( "tcDownSampleOffsets", dsOffsets, 16 );

   // render
   renderer->setRenderTarget( downsampleTarget );
   downsamplePass->beginRendering();
   renderQuad( data );
   downsamplePass->endRendering();
}

///////////////////////////////////////////////////////////////////////////////
