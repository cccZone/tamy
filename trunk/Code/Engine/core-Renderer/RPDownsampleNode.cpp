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
   : m_inputTex( NULL )
   , m_downsamplePass( NULL )
   , m_downsampleTarget( NULL )
   , m_renderer( NULL )
{
   defineInput( new RPTextureInput ( "InputTex" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPDownsampleNode::~RPDownsampleNode()
{
   delete m_downsamplePass;
   m_downsamplePass = NULL;

   m_inputTex = NULL;
   m_downsampleTarget = NULL;
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDownsampleNode::onInitialize( RenderingPipelineMechanism& host )
{
   __super::onInitialize( host );

   // aquire the input texture
   m_inputTex = getInput< RPTextureInput >( "InputTex" ).getTexture();
   m_downsampleTarget = getOutput< RPTextureOutput >( "Output" ).getRenderTarget();

   // initialize the pixel shader
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   m_downsamplePass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_DownSample4x4.psh" );
   m_downsamplePass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "DownSample4x4" );

   m_renderer = &host.getRenderer();
   m_renderer->implement< PixelShader >( *m_downsamplePass );
}

///////////////////////////////////////////////////////////////////////////////

void RPDownsampleNode::onDeinitialize( RenderingPipelineMechanism& host )
{
   delete m_downsamplePass;
   m_downsamplePass = NULL;

   m_inputTex = NULL;
   m_downsampleTarget = NULL;
   m_renderer = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPDownsampleNode::onUpdate( RenderingPipelineMechanism& host )
{
   if ( !m_inputTex )
   {
      return;
   }

   m_downsamplePass->setTexture( "inputTex", *m_inputTex );

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
            ( static_cast< float >( i ) + 0.5f ) * ( 1.0f / static_cast< float >( m_downsampleTarget->getWidth() ) ),
            ( static_cast< float >( j ) + 0.5f ) * ( 1.0f / static_cast< float >( m_downsampleTarget->getHeight() ) ),
            0.0f, // unused 
            0.0f  // unused
            );
      }
   }

   m_downsamplePass->setVec4Array( "tcDownSampleOffsets", dsOffsets, 16 );

   // render
   m_renderer->setRenderTarget( m_downsampleTarget );
   m_downsamplePass->beginRendering();
   renderQuad();
   m_downsamplePass->endRendering();
}

///////////////////////////////////////////////////////////////////////////////
