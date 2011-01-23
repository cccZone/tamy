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
}

///////////////////////////////////////////////////////////////////////////////

void RPHDRNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   __super::onCreateLayout( host );

   RuntimeDataBuffer& data = host.data();

   data.registerVar( m_hdrPass );
   data.registerVar( m_baseTex );
   data.registerVar( m_bloomedTex );
   data.registerVar( m_hdrTarget );
   data.registerVar( m_renderer );
}

///////////////////////////////////////////////////////////////////////////////

void RPHDRNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   __super::onInitialize( host );

   RuntimeDataBuffer& data = host.data();

   // acquire the input texture
   data[ m_baseTex ] = getInput< RPTextureInput >( "BaseTex" ).getTexture( data );
   data[ m_bloomedTex ] = getInput< RPTextureInput >( "BloomedTex" ).getTexture( data );
   data[ m_hdrTarget ] = getOutput< RPTextureOutput >( "Output" ).getRenderTarget( data );

   // initialize the pixel shader
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   data[ m_hdrPass ] = new PixelShader( "Renderer/Shaders/HDRPipeline/FinalPass.psh" );
   data[ m_hdrPass ]->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/FinalPass.psh", "main" );

   data[ m_renderer ] = &host.getRenderer();
   data[ m_renderer ]->implement< PixelShader >( *data[ m_hdrPass ] );
}

///////////////////////////////////////////////////////////////////////////////

void RPHDRNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   delete data[ m_hdrPass ];
   data[ m_hdrPass ] = NULL;

   data[ m_baseTex ] = NULL;
   data[ m_bloomedTex ] = NULL;
   data[ m_hdrTarget ] = NULL;
   data[ m_renderer ] = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPHDRNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   Renderer* renderer = data[ m_renderer ];
   PixelShader* hdrPass = data[ m_hdrPass ];
   ShaderTexture* baseTex = data[ m_baseTex ];
   ShaderTexture* bloomedTex = data[ m_bloomedTex ];
   RenderTarget* hdrTarget = data[ m_hdrTarget ];

   const RPFloatInput* maxLuminanceInput = &getInput< RPFloatInput >( "Luminance max" );
   const RPFloatInput* avgLuminanceInput = &getInput< RPFloatInput >( "Luminance avg" );

   if ( !baseTex || !bloomedTex || !maxLuminanceInput || !avgLuminanceInput )
   {
      return;
   }

   hdrPass->setTexture( "original_scene", *baseTex );
   hdrPass->setTexture( "bloom", *bloomedTex );

   hdrPass->setFloat( "maxLuminance", maxLuminanceInput->getValue( data ) );
   hdrPass->setFloat( "avgLuminance", avgLuminanceInput->getValue( data ) );

   hdrPass->setFloat( "g_rcp_bloom_tex_w", 1.0f / static_cast< float >( bloomedTex->getWidth() ) );
   hdrPass->setFloat( "g_rcp_bloom_tex_h", 1.0f / static_cast< float >( bloomedTex->getHeight() ) );
   hdrPass->setFloat( "fExposure", m_exposure );
   hdrPass->setFloat( "fGaussianScalar", m_gaussMultiplier );

   // render
   renderer->setRenderTarget( hdrTarget );
   hdrPass->beginRendering();
   renderQuad( data );
   hdrPass->endRendering();
}

///////////////////////////////////////////////////////////////////////////////
