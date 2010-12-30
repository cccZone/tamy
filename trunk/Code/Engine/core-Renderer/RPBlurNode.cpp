#include "core-Renderer/RPBlurNode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"

// TODO: enums editing

// TODO: some sort of validation of paths that go to the ResourcesManager - if we skip the initial slash (/),
// the resource doesn't get registered

// TODO: when exiting the main editor - all sub editors get closed as well

// TODO: !!!! adding a blur post process doesn't work - every thing's white


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBlurNode, RPPostProcessNode );
//   PROPERTY_EDIT( "Blur option", int, m_blurOption );
   PROPERTY_EDIT( "Gauss multiplier", float, m_gaussMultiplier );
   PROPERTY_EDIT( "Gauss mean", float, m_gaussMean );
   PROPERTY_EDIT( "Gauss standard deviation", float, m_gaussStdDev );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPBlurNode::RPBlurNode()
   : m_blurOption( BO_BOTH )
   , m_gaussMultiplier( 2.04f )
   , m_gaussMean( 0.0f )
   , m_gaussStdDev( 0.8f )
   , m_horizBlurPass( NULL )
   , m_vertBlurPass( NULL )
   , m_inputTex( NULL )
   , m_blurTarget( NULL )
   , m_tempBlurTarget( NULL )
   , m_renderer( NULL )
{
   defineInput( new RPTextureInput( "InputTex" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPBlurNode::~RPBlurNode()
{
   delete m_horizBlurPass;
   m_horizBlurPass = NULL;

   delete m_vertBlurPass;
   m_vertBlurPass = NULL;

   delete m_tempBlurTarget;
   m_tempBlurTarget = NULL;

   m_inputTex = NULL;
   m_blurTarget = NULL;
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPBlurNode::onInitialize( RenderingPipelineMechanism& host )
{
   __super::onInitialize( host );

   // aquire the input texture
   m_inputTex = getInput< RPTextureInput >( "InputTex" ).getTexture();
   m_blurTarget = getOutput< RPTextureOutput >( "Output" ).getRenderTarget();
   
   if( !m_inputTex )
   {
      return;
   }

   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   m_renderer = &host.getRenderer();

   switch( m_blurOption )
   {
   case BO_HORIZONTAL:
      {
         m_horizBlurPass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_HorizontalBlur.psh" );
         m_horizBlurPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "HorizontalBlur" );
         m_renderer->implement< PixelShader >( *m_horizBlurPass );

         break;
      }

   case BO_VERTICAL:
      {
         m_vertBlurPass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_VerticalBlur.psh" );
         m_vertBlurPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "VerticalBlur" );
         m_renderer->implement< PixelShader >( *m_vertBlurPass );

         break;
      }

   case BO_BOTH:
      {
         m_horizBlurPass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_HorizontalBlur.psh" );
         m_horizBlurPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "HorizontalBlur" );
         m_renderer->implement< PixelShader >( *m_horizBlurPass );

         m_vertBlurPass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_VerticalBlur.psh" );
         m_vertBlurPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "VerticalBlur" );
         m_renderer->implement< PixelShader >( *m_vertBlurPass );

         m_tempBlurTarget = new RenderTarget( new RTSPTexture( *m_renderer, *m_inputTex ) );
         m_renderer->implement< RenderTarget >( *m_tempBlurTarget );
         break;
      }
   }

}

///////////////////////////////////////////////////////////////////////////////

void RPBlurNode::onDeinitialize( RenderingPipelineMechanism& host )
{
   delete m_horizBlurPass;
   m_horizBlurPass = NULL;

   delete m_vertBlurPass;
   m_vertBlurPass = NULL;

   delete m_tempBlurTarget;
   m_tempBlurTarget = NULL;

   m_inputTex = NULL;
   m_blurTarget = NULL;
   m_renderer = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPBlurNode::onUpdate( RenderingPipelineMechanism& host )
{
   if ( !m_inputTex )
   {
      return;
   }

   // Configure the sampling offsets and their weights
   float bloomWeights[9];
   float bloomOffsets[9];

   for( int i = 0; i < 9; i++ )
   {
      // Compute the offsets. We take 9 samples - 4 either side and one in the middle:
      //     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
      //Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
      bloomOffsets[i] = ( static_cast< float >( i ) - 4.0f ) * ( 1.0f / static_cast< float >( m_inputTex->getWidth() ) );

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
         m_horizBlurPass->setTexture( "inputTex", *m_inputTex );

         m_horizBlurPass->setFloatArray( "HBloomWeights", bloomWeights, 9 );
         m_horizBlurPass->setFloatArray( "HBloomOffsets", bloomOffsets, 9 );

         // render
         m_renderer->setRenderTarget( m_blurTarget );
         m_horizBlurPass->beginRendering();
         renderQuad();
         m_horizBlurPass->endRendering();

         break;
      }

   case BO_VERTICAL:
      {
         m_vertBlurPass->setTexture( "inputTex", *m_inputTex );

         m_vertBlurPass->setFloatArray( "VBloomWeights", bloomWeights, 9 );
         m_vertBlurPass->setFloatArray( "VBloomOffsets", bloomOffsets, 9 );

         // render
         m_renderer->setRenderTarget( m_blurTarget );
         m_vertBlurPass->beginRendering();
         renderQuad();
         m_vertBlurPass->endRendering();

         break;
      }

   case BO_BOTH:
      {
         // horizontal blur
         {
            m_horizBlurPass->setTexture( "inputTex", *m_inputTex );

            m_horizBlurPass->setFloatArray( "HBloomWeights", bloomWeights, 9 );
            m_horizBlurPass->setFloatArray( "HBloomOffsets", bloomOffsets, 9 );

            // render
            m_renderer->setRenderTarget( m_tempBlurTarget );
            m_horizBlurPass->beginRendering();
            renderQuad();
            m_horizBlurPass->endRendering();
         }

         // vertical blur
         {
            m_vertBlurPass->setTexture( "inputTex", *m_tempBlurTarget );

            m_vertBlurPass->setFloatArray( "VBloomWeights", bloomWeights, 9 );
            m_vertBlurPass->setFloatArray( "VBloomOffsets", bloomOffsets, 9 );

            // render
            m_renderer->setRenderTarget( m_blurTarget );
            m_vertBlurPass->beginRendering();
            renderQuad();
            m_vertBlurPass->endRendering();
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
