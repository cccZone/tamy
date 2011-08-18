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
{
   defineInput( new RPTextureInput( "InputTex" ) );
   defineOutput( new RPFloatOutput( "Max" ) );
   defineOutput( new RPFloatOutput( "Avg" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPLuminanceNode::~RPLuminanceNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPLuminanceNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   __super::onCreateLayout( host );

   RuntimeDataBuffer& data = host.data();
   data.registerVar( m_renderer );
   data.registerVar( m_greyscalePass );
   data.registerVar( m_downSamplePass );
   data.registerVar( m_inputTex );
   data.registerVar( m_luminanceTarget );
   data.registerVar( m_luminanceVal );
}

///////////////////////////////////////////////////////////////////////////////

void RPLuminanceNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   __super::onInitialize( host );

   RuntimeDataBuffer& data = host.data();

   // acquire the input texture
   data[ m_inputTex ] = getInput< RPTextureInput >( "InputTex" ).getTexture( data );

   Renderer& renderer = host.getRenderer();
   data[ m_renderer ] = &renderer;

   // initialize the pixel shader
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   data[ m_greyscalePass ] = new PixelShader( "Renderer/Shaders/RenderingPipeline/Luminance_GreyScaleDownSample.psh" );
   data[ m_greyscalePass ]->loadFromFile( fs, "Renderer/Shaders/RenderingPipeline/Luminance.psh", "GreyScaleDownSample" );
   data[ m_greyscalePass ]->getParams().m_writeToZBuffer = false;
   data[ m_greyscalePass ]->getParams().m_useZBuffer = false;

   data[ m_downSamplePass ] = new PixelShader( "Renderer/Shaders/RenderingPipeline/Luminance_DownSample3x3.psh" );
   data[ m_downSamplePass ]->loadFromFile( fs, "Renderer/Shaders/RenderingPipeline/Luminance.psh", "DownSample3x3" );
   data[ m_downSamplePass ]->getParams().m_writeToZBuffer = false;
   data[ m_downSamplePass ]->getParams().m_useZBuffer = false;

   // luminance render targets
   PRenderTarget* luminanceTargets = new PRenderTarget[ m_stepsCount ];
   data[ m_luminanceTarget ] = luminanceTargets;

   int textureSize = 1;
   for ( unsigned int i = 0; i < m_stepsCount; ++i )
   {
      RenderTarget* target = new RenderTarget( new RTSPStatic( textureSize, textureSize ), TU_LUMINANCE, i == 0 /* we want to be able to read from the last RT */ );

      luminanceTargets[ m_stepsCount - i - 1 ] = target;
      textureSize *= 3;
   }

   data[ m_luminanceVal ] = Color();
}

///////////////////////////////////////////////////////////////////////////////

void RPLuminanceNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   delete data[ m_greyscalePass ];
   data[ m_greyscalePass ] = NULL;

   delete data[ m_downSamplePass ];
   data[ m_downSamplePass ] = NULL;

   PRenderTarget* luminanceTargets = data[ m_luminanceTarget ];
   for ( unsigned int i = 0; i < m_stepsCount; ++i )
   {
      delete luminanceTargets[i];
   }
   delete [] luminanceTargets;
   data[ m_luminanceTarget ] = NULL;

   data[ m_inputTex ] = NULL;
   data[ m_renderer ] = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPLuminanceNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();
   Renderer& renderer = *data[ m_renderer ];
   PixelShader* greyscalePass = data[ m_greyscalePass ];
   PixelShader* downSamplePass = data[ m_downSamplePass ];
   ShaderTexture* inputTex = data[ m_inputTex ];
   PRenderTarget* luminanceTargets = data[ m_luminanceTarget ];

   if ( !inputTex || m_stepsCount == 0 )
   {
      return;
   }

   // turn to greyscale and downsample
   {
      RCBindPixelShader* greyscalePassComm = new ( renderer() ) RCBindPixelShader( *greyscalePass );
      greyscalePassComm->setTexture( "inputTex", *inputTex );

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
      float sU = ( 1.0f / static_cast< float >( inputTex->getWidth() ) );
      float sV = ( 1.0f / static_cast< float >( inputTex->getHeight() ) );

      // The last two components (z,w) are unused. This makes for simpler code, but if
      // constant-storage is limited then it is possible to pack 4 offsets into 2 float4's
      offsets[0] = D3DXVECTOR4( -0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
      offsets[1] = D3DXVECTOR4( 0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
      offsets[2] = D3DXVECTOR4( -0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
      offsets[3] = D3DXVECTOR4( 0.5f * sU, -0.5f * sV, 0.0f, 0.0f );

      // Set the offsets to the constant table
      greyscalePassComm->setVec4( "tcLumOffsets", offsets, 4 );

      // render
      renderQuad( renderer, luminanceTargets[0] );
      new ( renderer() ) RCUnbindPixelShader( *greyscalePass );
   }

   // perform subsequent downscaling
   for ( unsigned int i = 1; i < m_stepsCount; ++i )
   {
      RCBindPixelShader* downSamplePassComm = new ( renderer() ) RCBindPixelShader( *downSamplePass );
      downSamplePassComm->setTexture( "inputTex", *luminanceTargets[i - 1] );

      // Create the 3x3 grid of offsets
      D3DXVECTOR4 DSoffsets[9];
      int idx = 0;
      for( int x = -1; x < 2; x++ )
      {
         for( int y = -1; y < 2; y++ )
         {
            DSoffsets[idx++] = D3DXVECTOR4(
               static_cast< float >( x ) / static_cast< float >( luminanceTargets[i - 1]->getWidth() ),
               static_cast< float >( y ) / static_cast< float >( luminanceTargets[i - 1]->getHeight() ),
               0.0f,   //unused
               0.0f    //unused
               );
         }
      }

      // Set them to the current pixel shader
      downSamplePassComm->setVec4( "tcDSOffsets", DSoffsets, 9 );

      // render
      renderQuad( renderer, luminanceTargets[i] );
      new ( renderer() ) RCUnbindPixelShader( *downSamplePass );
   }

   // set a command that will update the color value ( unfortunately - it will be available the next frame )
   Color& luminanceVal = data[ m_luminanceVal ];
   new ( renderer() ) RCGetPixel( *luminanceTargets[m_stepsCount - 1], D3DXVECTOR2( 0, 0 ), luminanceVal );

   // set the output values
   const RPFloatOutput* maxLuminanceOutput = &getOutput< RPFloatOutput >( "Max" );
   const RPFloatOutput* avgLuminanceOutput = &getOutput< RPFloatOutput >( "Avg" );

   if ( maxLuminanceOutput )
   {
      maxLuminanceOutput->setValue( data, luminanceVal.r );
   }
   if ( avgLuminanceOutput )
   {
      avgLuminanceOutput->setValue( data, luminanceVal.g );
   }
}

///////////////////////////////////////////////////////////////////////////////
