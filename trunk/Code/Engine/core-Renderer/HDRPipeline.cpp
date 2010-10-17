#include "core-Renderer\HDRPipeline.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\SceneRenderingPass.h"
#include "core-Renderer\VisibilityPass.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\PixelShader.h"
#include "core\ResourcesManager.h"
#include "core\Filesystem.h"


///////////////////////////////////////////////////////////////////////////////

HDRPipeline::HDRPipeline( ResourcesManager& rm, Camera& camera )
   : m_rm( rm )
   , m_camera( camera )
   , m_renderer( NULL )
   , m_sceneRenderTarget( NULL )
   , m_brightPassTarget( NULL )
   , m_downSampleTarget( NULL )
   , m_horizBlurTarget( NULL )
   , m_vertBlurTarget( NULL )
   , m_visibilityPass( NULL )
   , m_scenePass( NULL )
   , m_brightPass( NULL )
   , m_downSamplePass( NULL )
   , m_horizBlurPass( NULL )
   , m_vertBlurPass( NULL )
   , m_greyscalePass( NULL )
   , m_luminanceDownSamplePass( NULL )
   , m_hdrFinalPass( NULL )
   , m_fullscreenQuad( NULL )
   , m_brightThreshold( 0.8f )
   , m_gaussMultiplier( 0.4f )
   , m_gaussMean( 0.0f )
   , m_gaussStdDev( 0.8f )
   , m_exposure( 0.5f )
   , m_luminanceStepsCount( 6 )
{
}

///////////////////////////////////////////////////////////////////////////////

HDRPipeline::~HDRPipeline()
{
   m_renderer = NULL;

   delete m_visibilityPass;
   m_visibilityPass = NULL;

   delete m_scenePass;
   m_scenePass = NULL;

   delete m_brightPass;
   m_brightPass = NULL;

   delete m_downSamplePass;
   m_downSamplePass = NULL;

   delete m_horizBlurPass;
   m_horizBlurPass = NULL;

   delete m_vertBlurPass;
   m_vertBlurPass = NULL;

   delete m_greyscalePass;
   m_greyscalePass = NULL;

   delete m_luminanceDownSamplePass;
   m_luminanceDownSamplePass = NULL;

   delete m_hdrFinalPass;
   m_hdrFinalPass = NULL;

   delete m_sceneRenderTarget;
   m_sceneRenderTarget = NULL;

   delete m_brightPassTarget;
   m_brightPassTarget = NULL;

   delete m_horizBlurTarget;
   m_horizBlurTarget = NULL;

   delete m_vertBlurTarget;
   m_vertBlurTarget = NULL;

   for ( std::vector< RenderTarget* >::iterator it = m_luminanceTarget.begin(); it != m_luminanceTarget.end(); ++it )
   {
      delete *it;
   }
   m_luminanceTarget.clear();

   delete m_fullscreenQuad;
   m_fullscreenQuad = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void HDRPipeline::addScene( Model& scene )
{
   ASSERT_MSG( m_renderer != NULL, "Pipeline hasn't been implemented yet" );
   if ( m_renderer )
   {
      m_visibilityPass->addScene( scene );
      m_scenePass->addScene( scene );
   }
}

///////////////////////////////////////////////////////////////////////////////

void HDRPipeline::initialize( Renderer& renderer )
{
   // TODO: !!!!!!! wywalic renderowanie samej sceny do innego mechanizmu.
   // takie zas mechanizmy jak HDR niech korzystaja z produktow innych mechanizmow *( ktorymi sa tekstury z odpowiednim shitem )
   // WYSTAWIC mozliwosc graficznego konfigurowania pipeline'u

   m_renderer = &renderer;
   const Filesystem& fs = m_rm.getFilesystem();

   // visibility
   m_visibilityPass = new VisibilityPass( m_camera );
   m_visibilityPass->initialize( renderer );

   // scene
   m_sceneRenderTarget = new RenderTarget( new RTSPDynamic( renderer ) );
   renderer.implement< RenderTarget >( *m_sceneRenderTarget );
   
   m_scenePass = new SceneRenderingPass( m_sceneRenderTarget ) ;
   m_scenePass->initialize( renderer );

   // bright pass
   m_brightPassTarget = new RenderTarget( new RTSPDynamic( renderer, 0.5f, 0.5f ) );
   renderer.implement< RenderTarget >( *m_brightPassTarget );

   m_brightPass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_BrightPass.psh" );
   m_brightPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "BrightPass" );
   renderer.implement< PixelShader >( *m_brightPass );

   // downsample pass
   m_downSampleTarget = new RenderTarget( new RTSPDynamic( renderer, 0.125f, 0.125f ) );
   renderer.implement< RenderTarget >( *m_downSampleTarget );

   m_downSamplePass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_DownSample4x4.psh" );
   m_downSamplePass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "DownSample4x4" );
   renderer.implement< PixelShader >( *m_downSamplePass );

   // horizontal blur pass
   m_horizBlurTarget = new RenderTarget( new RTSPDynamic( renderer, 0.125f, 0.125f ) );
   renderer.implement< RenderTarget >( *m_horizBlurTarget );

   m_horizBlurPass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_HorizontalBlur.psh" );
   m_horizBlurPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "HorizontalBlur" );
   renderer.implement< PixelShader >( *m_horizBlurPass );

   // vertical blur pass
   m_vertBlurTarget = new RenderTarget( new RTSPDynamic( renderer, 0.125f, 0.125f ) );
   renderer.implement< RenderTarget >( *m_vertBlurTarget );

   m_vertBlurPass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postproces_sVerticalBlur.psh" );
   m_vertBlurPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "VerticalBlur" );
   renderer.implement< PixelShader >( *m_vertBlurPass );

   // luminance greyscale pass
   m_greyscalePass = new PixelShader( "Renderer/Shaders/HDRPipeline/Luminance_GreyScaleDownSample.psh" );
   m_greyscalePass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Luminance.psh", "GreyScaleDownSample" );
   renderer.implement< PixelShader >( *m_greyscalePass );

   // luminance downsampling pass
   m_luminanceDownSamplePass = new PixelShader( "Renderer/Shaders/HDRPipeline/Luminance_DownSample3x3.psh" );
   m_luminanceDownSamplePass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Luminance.psh", "DownSample3x3" );
   renderer.implement< PixelShader >( *m_luminanceDownSamplePass );

   // luminance render targets
   m_luminanceTarget.resize( m_luminanceStepsCount );
   int textureSize = 1;
   for ( unsigned int i = 0; i < m_luminanceStepsCount; ++i )
   {
      RenderTarget* target = new RenderTarget( new RTSPStatic( textureSize, textureSize), TU_LUMINANCE );
      renderer.implement< RenderTarget >( *target );

      m_luminanceTarget[ m_luminanceStepsCount - i - 1 ] = target;
      textureSize *= 3;
   }

   // hdr final pass
   m_hdrFinalPass = new PixelShader( "Renderer/Shaders/HDRPipeline/FinalPass.psh" );
   m_hdrFinalPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/FinalPass.psh", "main" );
   renderer.implement< PixelShader >( *m_hdrFinalPass );

   // fullscreen quad
   m_fullscreenQuad = new FullscreenQuad( &renderer );
   m_fullscreenQuad->initialize( m_rm );
   renderer.implement< FullscreenQuad >( *m_fullscreenQuad );
}

///////////////////////////////////////////////////////////////////////////////

void HDRPipeline::render()
{
   if ( !m_renderer )
   {
      return;
   }

   renderScene();
   measureLuminance();
   renderPostprocess();
   renderHDR();
}

///////////////////////////////////////////////////////////////////////////////

void HDRPipeline::renderScene()
{
   // scene pass
   m_visibilityPass->render();
   m_scenePass->render();
}

///////////////////////////////////////////////////////////////////////////////

void HDRPipeline::measureLuminance()
{
   unsigned int luminanceSamplesCount = m_luminanceTarget.size();
   if ( luminanceSamplesCount == 0 )
   {
      return;
   }

   // turn to greyscale and downsample
   {
      m_greyscalePass->setTexture( "inputTex", *m_sceneRenderTarget );
      
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
      float sU = ( 1.0f / static_cast< float >( m_sceneRenderTarget->getWidth() ) );
      float sV = ( 1.0f / static_cast< float >( m_sceneRenderTarget->getHeight() ) );

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
      m_fullscreenQuad->render();
      m_greyscalePass->endRendering();
   }

   // perform subsequent downscaling
   for ( unsigned int i = 1; i < luminanceSamplesCount; ++i )
   {
      m_luminanceDownSamplePass->setTexture( "inputTex", *m_luminanceTarget[i - 1] );

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
      m_luminanceDownSamplePass->setVec4Array( "tcDSOffsets", DSoffsets, 9 );

      // render
      m_renderer->setRenderTarget( m_luminanceTarget[i] );
      m_luminanceDownSamplePass->beginRendering();
      m_fullscreenQuad->render();
      m_luminanceDownSamplePass->endRendering();
   }
}

///////////////////////////////////////////////////////////////////////////////

void HDRPipeline::renderPostprocess()
{   
   // bright pass
   {
      m_brightPass->setTexture( "inputTex", *m_sceneRenderTarget );
      m_brightPass->setFloat( "fBrightPassThreshold", m_brightThreshold );


      // We need to compute the sampling offsets used for this pass.
      // A 2x2 sampling pattern is used, so we need to generate 4 offsets
      D3DXVECTOR4 offsets[4];

      // Because the source and destination are NOT the same sizes, we
      // need to provide offsets to correctly map between them.
      float sU = ( 1.0f / static_cast< float >( m_sceneRenderTarget->getWidth() ) );
      float sV = ( 1.0f / static_cast< float >( m_sceneRenderTarget->getHeight() ) );

      // The last two components (z,w) are unused. This makes for simpler code, but if
      // constant-storage is limited then it is possible to pack 4 offsets into 2 float4's
      offsets[0] = D3DXVECTOR4( -0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
      offsets[1] = D3DXVECTOR4( 0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
      offsets[2] = D3DXVECTOR4( -0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
      offsets[3] = D3DXVECTOR4( 0.5f * sU, -0.5f * sV, 0.0f, 0.0f );

      m_brightPass->setVec4Array( "tcDownSampleOffsets", offsets, 4 );

      // render
      m_renderer->setRenderTarget( m_brightPassTarget );
      m_brightPass->beginRendering();
      m_fullscreenQuad->render();
      m_brightPass->endRendering();
   }

   // down sample
   {
      m_downSamplePass->setTexture( "inputTex", *m_brightPassTarget );

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
               ( static_cast< float >( i ) + 0.5f ) * ( 1.0f / static_cast< float >( m_downSampleTarget->getWidth() ) ),
               ( static_cast< float >( j ) + 0.5f ) * ( 1.0f / static_cast< float >( m_downSampleTarget->getHeight() ) ),
               0.0f, // unused 
               0.0f  // unused
               );
         }
      }

      m_downSamplePass->setVec4Array( "tcDownSampleOffsets", dsOffsets, 16 );

      // render
      m_renderer->setRenderTarget( m_downSampleTarget );
      m_downSamplePass->beginRendering();
      m_fullscreenQuad->render();
      m_downSamplePass->endRendering();
   }

   // blurring
   {
      // Configure the sampling offsets and their weights
      float bloomWeights[9];
      float bloomOffsets[9];

      for( int i = 0; i < 9; i++ )
      {
         // Compute the offsets. We take 9 samples - 4 either side and one in the middle:
         //     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
         //Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
         bloomOffsets[i] = ( static_cast< float >( i ) - 4.0f ) * ( 1.0f / static_cast< float >( m_downSampleTarget->getWidth() ) );

         // 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
         float x = ( static_cast< float >( i ) - 4.0f ) / 4.0f;

         // Use a gaussian distribution. Changing the standard-deviation
         // (second parameter) as well as the amplitude (multiplier) gives
         // distinctly different results.
         bloomWeights[i] = m_gaussMultiplier * computeGaussianValue( x, m_gaussMean, m_gaussStdDev );
      }

      // horizontal blur
      {
         m_horizBlurPass->setTexture( "inputTex", *m_downSampleTarget );

         m_horizBlurPass->setFloatArray( "HBloomWeights", bloomWeights, 9 );
         m_horizBlurPass->setFloatArray( "HBloomOffsets", bloomOffsets, 9 );

         // render
         m_renderer->setRenderTarget( m_horizBlurTarget );
         m_horizBlurPass->beginRendering();
         m_fullscreenQuad->render();
         m_horizBlurPass->endRendering();
      }

      // vertical blur
      {
         m_vertBlurPass->setTexture( "inputTex", *m_horizBlurTarget );


         m_vertBlurPass->setFloatArray( "VBloomWeights", bloomWeights, 9 );
         m_vertBlurPass->setFloatArray( "VBloomOffsets", bloomOffsets, 9 );

         // render
         m_renderer->setRenderTarget( m_vertBlurTarget );
         m_vertBlurPass->beginRendering();
         m_fullscreenQuad->render();
         m_vertBlurPass->endRendering();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

float HDRPipeline::computeGaussianValue( float x, float mean, float std_deviation ) const
{
    // The gaussian equation is defined as such:
    /*    
      -(x - mean)^2
      -------------
      1.0               2*std_dev^2
      f(x,mean,std_dev) = -------------------- * e^
      sqrt(2*pi*std_dev^2)
      
     */

    return ( 1.0f / sqrt( 2.0f * D3DX_PI * std_deviation * std_deviation ) )
        * expf( ( -( ( x - mean ) * ( x - mean ) ) ) / ( 2.0f * std_deviation * std_deviation ) );
}

///////////////////////////////////////////////////////////////////////////////

void HDRPipeline::renderHDR()
{
   m_hdrFinalPass->setTexture( "original_scene", *m_sceneRenderTarget );
   m_hdrFinalPass->setTexture( "luminance", *m_luminanceTarget.back() );
   m_hdrFinalPass->setTexture( "bloom", *m_vertBlurTarget );

   m_hdrFinalPass->setFloat( "g_rcp_bloom_tex_w", 1.0f / static_cast< float >( m_vertBlurTarget->getWidth() ) );
   m_hdrFinalPass->setFloat( "g_rcp_bloom_tex_h", 1.0f / static_cast< float >( m_vertBlurTarget->getHeight() ) );
   m_hdrFinalPass->setFloat( "fExposure", m_exposure );
   m_hdrFinalPass->setFloat( "fGaussianScalar", m_gaussMultiplier );

   // render
   m_renderer->setRenderTarget( NULL );
   m_hdrFinalPass->beginRendering();
   m_fullscreenQuad->render();
   m_hdrFinalPass->endRendering();
}

///////////////////////////////////////////////////////////////////////////////
