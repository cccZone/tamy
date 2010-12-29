#include "core-Renderer/RPHDRNode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/FloatSockets.h"
#include "core-Renderer/ShaderTexture.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPHDRNode, RPPostProcessNode )
   PROPERTY_EDIT( "Exposure", float, m_exposure )
   PROPERTY_EDIT( "Gauss multiplier", float, m_gaussMultiplier )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPHDRNode::RPHDRNode()
   : m_exposure( 0.8f )
   , m_gaussMultiplier( 2.04f )
   , m_hdrPass( NULL )
   , m_baseTex( NULL )
   , m_bloomedTex( NULL )
   , m_maxLuminanceInput( NULL )
   , m_avgLuminanceInput( NULL )
   , m_hdrTarget( NULL )
   , m_renderer( NULL )
{
   defineInput( new RPTextureInput( "BaseTex" ) );
   defineInput( new RPTextureInput( "BloomedTex" ) );
   defineInput( new RPFloatInput( "Luminance max" ) );
   defineInput( new RPFloatInput( "Luminance avg" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPHDRNode::~RPHDRNode()
{
   delete m_hdrPass;
   m_hdrPass = NULL;

   m_baseTex = NULL;
   m_bloomedTex = NULL;
   m_maxLuminanceInput = NULL;
   m_avgLuminanceInput = NULL;
   m_hdrTarget = NULL;
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPHDRNode::onInitialize( RenderingPipelineMechanism& host )
{
   __super::onInitialize( host );

   // aquire the input texture
   m_baseTex = getInput< RPTextureInput >( "BaseTex" ).getTexture();
   m_bloomedTex = getInput< RPTextureInput >( "BloomedTex" ).getTexture();
   m_maxLuminanceInput = &getInput< RPFloatInput >( "Luminance max" );
   m_avgLuminanceInput = &getInput< RPFloatInput >( "Luminance avg" );
   m_hdrTarget = getOutput< RPTextureOutput >( "Output" ).getRenderTarget();

   // initialize the pixel shader
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   m_hdrPass = new PixelShader( "Renderer/Shaders/HDRPipeline/FinalPass.psh" );
   m_hdrPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/FinalPass.psh", "main" );

   m_renderer = &host.getRenderer();
   m_renderer->implement< PixelShader >( *m_hdrPass );
}

///////////////////////////////////////////////////////////////////////////////

void RPHDRNode::onDeinitialize( RenderingPipelineMechanism& host )
{
   delete m_hdrPass;
   m_hdrPass = NULL;

   m_baseTex = NULL;
   m_bloomedTex = NULL;
   m_maxLuminanceInput = NULL;
   m_avgLuminanceInput = NULL;
   m_hdrTarget = NULL;
   m_renderer = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPHDRNode::onUpdate( RenderingPipelineMechanism& host )
{
   if ( !m_baseTex || !m_bloomedTex || !m_maxLuminanceInput || !m_avgLuminanceInput )
   {
      return;
   }

   m_hdrPass->setTexture( "original_scene", *m_baseTex );
   m_hdrPass->setTexture( "bloom", *m_bloomedTex );

   m_hdrPass->setFloat( "maxLuminance", m_maxLuminanceInput->getValue() );
   m_hdrPass->setFloat( "avgLuminance", m_avgLuminanceInput->getValue() );

   m_hdrPass->setFloat( "g_rcp_bloom_tex_w", 1.0f / static_cast< float >( m_bloomedTex->getWidth() ) );
   m_hdrPass->setFloat( "g_rcp_bloom_tex_h", 1.0f / static_cast< float >( m_bloomedTex->getHeight() ) );
   m_hdrPass->setFloat( "fExposure", m_exposure );
   m_hdrPass->setFloat( "fGaussianScalar", m_gaussMultiplier );

   // render
   m_renderer->setRenderTarget( m_hdrTarget );
   m_hdrPass->beginRendering();
   renderQuad();
   m_hdrPass->endRendering();
}

///////////////////////////////////////////////////////////////////////////////
