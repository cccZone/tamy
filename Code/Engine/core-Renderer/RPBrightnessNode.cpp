#include "core-Renderer/RPBrightnessNode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/ShaderTexture.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBrightnessNode, RPPostProcessNode )
   PROPERTY_EDIT( "Threshold", float, m_brightThreshold )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

RPBrightnessNode::RPBrightnessNode()
: m_brightThreshold( 0.8f )
, m_brightnessPass( NULL )
, m_inputTex( NULL )
, m_brightPassTarget( NULL )
, m_renderer( NULL )
{
   defineInput( new RPTextureInput( "InputTex" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPBrightnessNode::~RPBrightnessNode()
{
   delete m_brightnessPass;
   m_brightnessPass = NULL;

   m_inputTex = NULL;
   m_brightPassTarget = NULL;
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPBrightnessNode::onInitialize( RenderingPipelineMechanism& host )
{
   __super::onInitialize( host );

   // aquire the input texture
   m_inputTex = getInput< RPTextureInput >( "InputTex" ).getTexture();
   m_brightPassTarget = getOutput< RPTextureOutput >( "Output" ).getRenderTarget();

   // initialize the pixel shader
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   m_brightnessPass = new PixelShader( "Renderer/Shaders/HDRPipeline/Postprocess_BrightPassDownSample.psh" );
   m_brightnessPass->loadFromFile( fs, "Renderer/Shaders/HDRPipeline/Postprocess.psh", "BrightPassDownSample" );

   m_renderer = &host.getRenderer();
   m_renderer->implement< PixelShader >( *m_brightnessPass );
}

///////////////////////////////////////////////////////////////////////////////

void RPBrightnessNode::onDeinitialize( RenderingPipelineMechanism& host )
{
   delete m_brightnessPass;
   m_brightnessPass = NULL;

   m_inputTex = NULL;
   m_brightPassTarget = NULL;
   m_renderer = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPBrightnessNode::onUpdate( RenderingPipelineMechanism& host )
{
   if ( !m_inputTex )
   {
      return;
   }

   m_brightnessPass->setTexture( "inputTex", *m_inputTex );
   m_brightnessPass->setFloat( "fBrightPassThreshold", m_brightThreshold );

   // We need to compute the sampling offsets used for this pass.
   // A 2x2 sampling pattern is used, so we need to generate 4 offsets
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

   m_brightnessPass->setVec4Array( "tcDownSampleOffsets", offsets, 4 );

   // render
   m_renderer->setRenderTarget( m_brightPassTarget );
   m_brightnessPass->beginRendering();
   renderQuad();
   m_brightnessPass->endRendering();
}

///////////////////////////////////////////////////////////////////////////////
