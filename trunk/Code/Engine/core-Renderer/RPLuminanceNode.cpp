#include "core-Renderer/RPLuminanceNode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/FloatSockets.h"
#include "core-Renderer/RenderTarget.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPLuminanceNode, RPPostProcessNode )
   PROPERTY_EDIT( "Downsample steps", unsigned int, m_stepsCount )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPLuminanceNode::RPLuminanceNode()
   : m_stepsCount( 6 )
   , m_greyscalePass( NULL )
   , m_downSamplePass( NULL )
   , m_inputTex( NULL )
   , m_renderer( NULL )
   , m_maxLuminanceOutput( NULL )
   , m_avgLuminanceOutput( NULL )
{
   defineInput( new RPTextureInput( "InputTex" ) );
   defineOutput( new RPFloatOutput( "Max" ) );
   defineOutput( new RPFloatOutput( "Avg" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPLuminanceNode::~RPLuminanceNode()
{
   delete m_greyscalePass;
   m_greyscalePass = NULL;

   delete m_downSamplePass;
   m_downSamplePass = NULL;

   for ( std::vector< RenderTarget* >::iterator it = m_luminanceTarget.begin(); it != m_luminanceTarget.end(); ++it )
   {
      delete *it;
   }
   m_luminanceTarget.clear();

   m_inputTex = NULL;
   m_renderer = NULL;

   m_maxLuminanceOutput = NULL;
   m_avgLuminanceOutput = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPLuminanceNode::onInitialize( RenderingPipelineMechanism& host )
{
   __super::onInitialize( host );

   // aquire the input texture
   m_inputTex = getInput< RPTextureInput >( "InputTex" ).getTexture();
   m_maxLuminanceOutput = &getOutput< RPFloatOutput >( "Max" );
   m_avgLuminanceOutput = &getOutput< RPFloatOutput >( "Avg" );

   // initialize the pixel shader
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   m_greyscalePass = new PixelShader( "Renderer/Shaders/HDRPipeline/Luminance_GreyScaleDownSample.psh" );
   m_greyscalePass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Luminance.psh", "GreyScaleDownSample" );

   m_downSamplePass = new PixelShader( "Renderer/Shaders/HDRPipeline/Luminance_DownSample3x3.psh" );
   m_downSamplePass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Luminance.psh", "DownSample3x3" );

   m_renderer = &host.getRenderer();
   m_renderer->implement< PixelShader >( *m_greyscalePass );
   m_renderer->implement< PixelShader >( *m_downSamplePass );

   // luminance render targets
   m_luminanceTarget.resize( m_stepsCount );
   int textureSize = 1;
   for ( unsigned int i = 0; i < m_stepsCount; ++i )
   {
      RenderTarget* target = new RenderTarget( new RTSPStatic( textureSize, textureSize), TU_LUMINANCE, i == 0 /* we want to be able to read from the last RT */ );
      m_renderer->implement< RenderTarget >( *target );

      m_luminanceTarget[ m_stepsCount - i - 1 ] = target;
      textureSize *= 3;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPLuminanceNode::onDeinitialize( RenderingPipelineMechanism& host )
{
   delete m_greyscalePass;
   m_greyscalePass = NULL;

   delete m_downSamplePass;
   m_downSamplePass = NULL;

   for ( std::vector< RenderTarget* >::iterator it = m_luminanceTarget.begin(); it != m_luminanceTarget.end(); ++it )
   {
      delete *it;
   }
   m_luminanceTarget.clear();

   m_inputTex = NULL;
   m_renderer = NULL;

   m_maxLuminanceOutput = NULL;
   m_avgLuminanceOutput = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPLuminanceNode::onUpdate( RenderingPipelineMechanism& host )
{
   if ( !m_inputTex || m_stepsCount == 0 || ( !m_maxLuminanceOutput && !m_avgLuminanceOutput ) )
   {
      return;
   }

   // turn to greyscale and downsample
   {
      m_greyscalePass->setTexture( "inputTex", *m_inputTex );

      // We need to compute the sampling offsets used for this pass.
      // A 2x2 sampling pattern is used, so we need to generate 4 offsets.
      //
      // NOTE: It is worth noting that some information will likely be lost
      //       due to the luminance map being less than 1/2 the size of the
      //       original render-target. This mis-match does not have a particularly
      //       big impact on the final luminance measurement. If necessary,
      //       the same process could be used - but with many more samples, so as
      //       to correctly map from HDR->Luminance without losing information.
      D3DXVECTOR4 offsets[4];

      // Because the source and destination are NOT the same sizes, we
      // need to provide offsets to correctly map between them.
      float sU = ( 1.0f / static_cast< float >( m_inputTex->getWidth() ) );
      float sV = ( 1.0f / static_cast< float >( m_inputTex->getHeight() ) );

      // The last two components (z,w) are unused. This makes for simpler code, but if
      // constant-storage is limited then it is possible to pack 4 offsets into 2 float4's
      offsets[0] = D3DXVECTOR4( -0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
      offsets[1] = D3DXVECTOR4( 0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
      offsets[2] = D3DXVECTOR4( -0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
      offsets[3] = D3DXVECTOR4( 0.5f * sU, -0.5f * sV, 0.0f, 0.0f );

      // Set the offsets to the constant table
      m_greyscalePass->setVec4Array( "tcLumOffsets", offsets, 4 );

      // render
      m_renderer->setRenderTarget( m_luminanceTarget[0] );
      m_greyscalePass->beginRendering();
      renderQuad();
      m_greyscalePass->endRendering();
   }

   // perform subsequent downscaling
   for ( unsigned int i = 1; i < m_stepsCount; ++i )
   {
      m_downSamplePass->setTexture( "inputTex", *m_luminanceTarget[i - 1] );

      // Create the 3x3 grid of offsets
      D3DXVECTOR4 DSoffsets[9];
      int idx = 0;
      for( int x = -1; x < 2; x++ )
      {
         for( int y = -1; y < 2; y++ )
         {
            DSoffsets[idx++] = D3DXVECTOR4(
               static_cast< float >( x ) / static_cast< float >( m_luminanceTarget[i - 1]->getWidth() ),
               static_cast< float >( y ) / static_cast< float >( m_luminanceTarget[i - 1]->getHeight() ),
               0.0f,   //unused
               0.0f    //unused
               );
         }
      }

      // Set them to the current pixel shader
      m_downSamplePass->setVec4Array( "tcDSOffsets", DSoffsets, 9 );

      // render
      m_renderer->setRenderTarget( m_luminanceTarget[i] );
      m_downSamplePass->beginRendering();
      renderQuad();
      m_downSamplePass->endRendering();
   }

   Color luminanceVal = m_luminanceTarget.back()->getPixel( D3DXVECTOR2( 0, 0 ) );
   if ( m_maxLuminanceOutput )
   {
      m_maxLuminanceOutput->setValue( luminanceVal.r );
   }
   if ( m_avgLuminanceOutput )
   {
      m_avgLuminanceOutput->setValue( luminanceVal.g );
   }
}

///////////////////////////////////////////////////////////////////////////////
