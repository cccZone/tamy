#include "core-Renderer/RPBrightnessNode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/ShaderTexture.h"
#include "core-Renderer/RenderTarget.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBrightnessNode, RPPostProcessNode )
   PROPERTY_EDIT( "Threshold", float, m_brightThreshold )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPBrightnessNode::RPBrightnessNode()
: m_brightThreshold( 0.8f )
{
   defineInput( new RPTextureInput( "InputTex" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPBrightnessNode::~RPBrightnessNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPBrightnessNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   __super::onCreateLayout( host );

   RuntimeDataBuffer& data = host.data();

   data.registerVar( m_brightnessPass );
   data.registerVar( m_inputTex );
   data.registerVar( m_brightPassTarget );
   data.registerVar( m_renderer );
}

///////////////////////////////////////////////////////////////////////////////

void RPBrightnessNode::onInitialize( RenderingPipelineMechanism& host ) const
{
   __super::onInitialize( host );

   RuntimeDataBuffer& data = host.data();

   // acquire the input texture
   data[ m_inputTex ] = getInput< RPTextureInput >( "InputTex" ).getTexture( data );
   data[ m_brightPassTarget ] = getOutput< RPTextureOutput >( "Output" ).getRenderTarget( data );

   // initialize the pixel shader
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   data[ m_brightnessPass ] = new PixelShader( "Renderer/Shaders/RenderingPipeline/Postprocess_BrightPassDownSample.psh" );
   data[ m_brightnessPass ]->loadFromFile( fs, "Renderer/Shaders/RenderingPipeline/Postprocess.psh", "BrightPassDownSample" );
   data[ m_brightnessPass ]->getParams().m_writeToZBuffer = false;
   data[ m_brightnessPass ]->getParams().m_useZBuffer = false;

   data[ m_renderer ] = &host.getRenderer();
   data[ m_renderer ]->implement< PixelShader >( *data[ m_brightnessPass ] );
}

///////////////////////////////////////////////////////////////////////////////

void RPBrightnessNode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   delete data[ m_brightnessPass ];
   data[ m_brightnessPass ] = NULL;

   data[ m_inputTex ] = NULL;
   data[ m_brightPassTarget ] = NULL;
   data[ m_renderer ] = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPBrightnessNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   Renderer* renderer = data[ m_renderer ];
   PixelShader* brightnessPass = data[ m_brightnessPass ];
   ShaderTexture* inputTex = data[ m_inputTex ];
   RenderTarget* brightPassTarget = data[ m_brightPassTarget ];

   if ( !inputTex )
   {
      return;
   }

   brightnessPass->setTexture( "inputTex", *inputTex );
   brightnessPass->setFloat( "fBrightPassThreshold", m_brightThreshold );

   // We need to compute the sampling offsets used for this pass.
   // A 2x2 sampling pattern is used, so we need to generate 4 offsets
   D3DXVECTOR4 offsets[4];

   // Because the source and destination are NOT the same sizes, we
   // need to provide offsets to correctly map between them.
   unsigned int texWidth = inputTex->getWidth();
   unsigned int texHeight = inputTex->getHeight();
   float sU = ( 1.0f / static_cast< float >( texWidth ) );
   float sV = ( 1.0f / static_cast< float >( texHeight ) );

   // The last two components (z,w) are unused. This makes for simpler code, but if
   // constant-storage is limited then it is possible to pack 4 offsets into 2 float4's
   offsets[0] = D3DXVECTOR4( -0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
   offsets[1] = D3DXVECTOR4( 0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
   offsets[2] = D3DXVECTOR4( -0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
   offsets[3] = D3DXVECTOR4( 0.5f * sU, -0.5f * sV, 0.0f, 0.0f );

   brightnessPass->setVec4Array( "tcDownSampleOffsets", offsets, 4 );


   // render
   brightnessPass->beginRendering();
   renderQuad( data, *renderer, brightPassTarget );
   brightnessPass->endRendering();
}

///////////////////////////////////////////////////////////////////////////////
