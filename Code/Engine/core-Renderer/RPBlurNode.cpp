#include "core-Renderer/RPBlurNode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBlurNode, RPPostProcessNode );
   PROPERTY_EDIT( "Blur option", BlurOptions, m_blurOption );
   PROPERTY_EDIT( "Gauss multiplier", float, m_gaussMultiplier );
   PROPERTY_EDIT( "Gauss mean", float, m_gaussMean );
   PROPERTY_EDIT( "Gauss standard deviation", float, m_gaussStdDev );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

BEGIN_ENUM( BlurOptions );
   ENUM_VAL( BO_HORIZONTAL );
   ENUM_VAL( BO_VERTICAL );
   ENUM_VAL( BO_BOTH );
END_ENUM( BlurOptions );

///////////////////////////////////////////////////////////////////////////////

RPBlurNode::RPBlurNode()
   : m_blurOption( BO_BOTH )
   , m_gaussMultiplier( 2.04f )
   , m_gaussMean( 0.0f )
   , m_gaussStdDev( 0.8f )
{
   defineInput( new RPTextureInput( "InputTex" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPBlurNode::~RPBlurNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPBlurNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   __super::onCreateLayout( host );

   RuntimeDataBuffer& data = host.data();

   data.registerVar( m_renderer );
   data.registerVar( m_horizBlurPass );
   data.registerVar( m_vertBlurPass );
   data.registerVar( m_inputTex );
   data.registerVar( m_blurTarget );
   data.registerVar( m_tempBlurTarget );
}

///////////////////////////////////////////////////////////////////////////////

void RPBlurNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   __super::onInitialize( host );

   RuntimeDataBuffer& data = host.data();

   // acquire the input texture
   data[ m_inputTex ] = getInput< RPTextureInput >( "InputTex" ).getTexture( data );
   data[ m_blurTarget ] = getOutput< RPTextureOutput >( "Output" ).getRenderTarget( data );
   
   if( !data[ m_inputTex ] )
   {
      return;
   }

   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   Renderer& renderer = host.getRenderer();
   data[ m_renderer ] = &renderer;

   switch( m_blurOption )
   {
   case BO_HORIZONTAL:
      {
         data[ m_horizBlurPass ] = new PixelShader( "Renderer/Shaders/RenderingPipeline/Postprocess_HorizontalBlur.psh" );
         data[ m_horizBlurPass ]->loadFromFile( fs, "Renderer/Shaders/RenderingPipeline/Postprocess.psh", "HorizontalBlur" );
         data[ m_horizBlurPass ]->getParams().m_writeToZBuffer = false;
         data[ m_horizBlurPass ]->getParams().m_useZBuffer = false;

         break;
      }

   case BO_VERTICAL:
      {
         data[ m_vertBlurPass ] = new PixelShader( "Renderer/Shaders/RenderingPipeline/Postprocess_VerticalBlur.psh" );
         data[ m_vertBlurPass ]->loadFromFile( fs, "Renderer/Shaders/RenderingPipeline/Postprocess.psh", "VerticalBlur" );
         data[ m_vertBlurPass ]->getParams().m_writeToZBuffer = false;
         data[ m_vertBlurPass ]->getParams().m_useZBuffer = false;

         break;
      }

   case BO_BOTH:
      {
         data[ m_horizBlurPass ] = new PixelShader( "Renderer/Shaders/RenderingPipeline/Postprocess_HorizontalBlur.psh" );
         data[ m_horizBlurPass ]->loadFromFile( fs, "Renderer/Shaders/RenderingPipeline/Postprocess.psh", "HorizontalBlur" );
         data[ m_horizBlurPass ]->getParams().m_writeToZBuffer = false;
         data[ m_horizBlurPass ]->getParams().m_useZBuffer = false;

         data[ m_vertBlurPass ] = new PixelShader( "Renderer/Shaders/RenderingPipeline/Postprocess_VerticalBlur.psh" );
         data[ m_vertBlurPass ]->loadFromFile( fs, "Renderer/Shaders/RenderingPipeline/Postprocess.psh", "VerticalBlur" );
         data[ m_vertBlurPass ]->getParams().m_writeToZBuffer = false;
         data[ m_vertBlurPass ]->getParams().m_useZBuffer = false;

         data[ m_tempBlurTarget ] = new RenderTarget( new RTSPTexture( *data[ m_renderer ], *data[ m_inputTex ] ) );
         break;
      }
   }

}

///////////////////////////////////////////////////////////////////////////////

void RPBlurNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   delete data[ m_horizBlurPass ];
   data[ m_horizBlurPass ] = NULL;

   delete data[ m_vertBlurPass ];
   data[ m_vertBlurPass ] = NULL;

   delete data[ m_tempBlurTarget ];
   data[ m_tempBlurTarget  ] = NULL;

   data[ m_inputTex ] = NULL;
   data[ m_blurTarget ] = NULL;
   data[ m_renderer ] = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPBlurNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   Renderer& renderer = *data[ m_renderer ];
   PixelShader* horizBlurPass = data[ m_horizBlurPass ];
   PixelShader* vertBlurPass = data[ m_vertBlurPass ];
   ShaderTexture* inputTex = data[ m_inputTex ];
   RenderTarget* blurTarget = data[ m_blurTarget ];
   RenderTarget* tempBlurTarget = data[ m_tempBlurTarget ];

   if ( !inputTex )
   {
      return;
   }

   // Configure the sampling offsets and their weights
   float bloomWeights[9];
   float horizBloomOffsets[9];
   float vertBloomOffsets[9];

   // get target texture sizes
   unsigned int dstWidth, dstHeight;
   if ( blurTarget )
   {
      dstWidth = blurTarget->getWidth();
      dstHeight = blurTarget->getHeight();
   }
   else
   {
      dstWidth = renderer.getViewportWidth();
      dstHeight = renderer.getViewportHeight();
   }

   float widthInv = 1.0f / (float)dstWidth;
   float heightInv = 1.0f /(float)dstHeight;

   for( int i = 0; i < 9; i++ )
   {
      // Compute the offsets. We take 9 samples - 4 either side and one in the middle:
      //     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
      //Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
      horizBloomOffsets[i] = ( static_cast< float >( i ) - 4.0f ) * widthInv;
      vertBloomOffsets[i] = ( static_cast< float >( i ) - 4.0f ) * heightInv;

      // 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
      float x = ( static_cast< float >( i ) - 4.0f ) / 4.0f;

      // Use a gaussian distribution. Changing the standard-deviation
      // (second parameter) as well as the amplitude (multiplier) gives
      // distinctly different results.
      bloomWeights[i] = m_gaussMultiplier * computeGaussianValue( x, m_gaussMean, m_gaussStdDev );
   }

   switch( m_blurOption )
   {
   case BO_HORIZONTAL:
      {
         RCBindPixelShader* horizBlurPassComm = new ( renderer() ) RCBindPixelShader( *horizBlurPass );
         horizBlurPassComm->setTexture( "inputTex", *inputTex );

         horizBlurPassComm->setFloat( "HBloomWeights", bloomWeights, 9 );
         horizBlurPassComm->setFloat( "HBloomOffsets", horizBloomOffsets, 9 );

         // render
         renderQuad( renderer, blurTarget );
         new ( renderer() ) RCUnbindPixelShader( *horizBlurPass );

         break;
      }

   case BO_VERTICAL:
      {
         RCBindPixelShader* vertBlurPassComm = new ( renderer() ) RCBindPixelShader( *vertBlurPass );
         vertBlurPassComm->setTexture( "inputTex", *inputTex );

         vertBlurPassComm->setFloat( "VBloomWeights", bloomWeights, 9 );
         vertBlurPassComm->setFloat( "VBloomOffsets", vertBloomOffsets, 9 );

         // render
         renderQuad( renderer, blurTarget );
         new ( renderer() ) RCUnbindPixelShader( *vertBlurPass );

         break;
      }

   case BO_BOTH:
      {
         // horizontal blur
         {
            RCBindPixelShader* horizBlurPassComm = new ( renderer() ) RCBindPixelShader( *horizBlurPass );
            horizBlurPassComm->setTexture( "inputTex", *inputTex );

            horizBlurPassComm->setFloat( "HBloomWeights", bloomWeights, 9 );
            horizBlurPassComm->setFloat( "HBloomOffsets", horizBloomOffsets, 9 );

            // render
            renderQuad( renderer, tempBlurTarget );
            new ( renderer() ) RCUnbindPixelShader( *horizBlurPass );
         }

         // vertical blur
         {
            RCBindPixelShader* vertBlurPassComm = new ( renderer() ) RCBindPixelShader( *vertBlurPass );
            vertBlurPassComm->setTexture( "inputTex", *tempBlurTarget );

            vertBlurPassComm->setFloat( "VBloomWeights", bloomWeights, 9 );
            vertBlurPassComm->setFloat( "VBloomOffsets", vertBloomOffsets, 9 );

            // render
            renderQuad( renderer, blurTarget );
            new ( renderer() ) RCUnbindPixelShader( *vertBlurPass );
         }

         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

float RPBlurNode::computeGaussianValue( float x, float mean, float std_deviation ) const
{
    // The gaussian equation is defined as such:
    /*    
                                                   -(x - mean)^2
                                                   -------------
                                   1.0              2*std_dev^2
      f(x,mean,std_dev) = -------------------- * e^
                           sqrt(2*pi*std_dev^2)
      
     */

    return ( 1.0f / sqrt( 2.0f * D3DX_PI * std_deviation * std_deviation ) )
        * expf( ( -( ( x - mean ) * ( x - mean ) ) ) / ( 2.0f * std_deviation * std_deviation ) );
}

///////////////////////////////////////////////////////////////////////////////
