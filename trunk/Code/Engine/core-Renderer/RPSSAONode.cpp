#include "core-Renderer/RPSSAONode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/Texture.h"
#include "core-Renderer/Defines.h"
#include "core-Renderer/Camera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSSAONode, RPPostProcessNode );
   PROPERTY_EDIT( "Radius", float, m_occlusionRadius );
   PROPERTY_EDIT( "Sampling noise", Texture*, m_samplingPatternNoise );
END_OBJECT();


///////////////////////////////////////////////////////////////////////////////

RPSSAONode::RPSSAONode()
   : m_occlusionRadius( 1.0f )
   , m_samplingPatternNoise( NULL )
{
   defineInput( new RPTextureInput( "Normals&Depth" ) );
   defineOutput( new RPTextureOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

RPSSAONode::~RPSSAONode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPSSAONode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   __super::onCreateLayout( host );

   RuntimeDataBuffer& data = host.data();

   data.registerVar( m_renderer );
   data.registerVar( m_ssaoPass );
   data.registerVar( m_normalsAndDepthBuffer );
   data.registerVar( m_outputTarget );
}

///////////////////////////////////////////////////////////////////////////////

void RPSSAONode::onInitialize( RenderingPipelineMechanism& host ) const
{
   __super::onInitialize( host );

   RuntimeDataBuffer& data = host.data();

   // acquire the input texture
   data[ m_normalsAndDepthBuffer ] = getInput< RPTextureInput >( "Normals&Depth" ).getTexture( data );
   data[ m_outputTarget ] = getOutput< RPTextureOutput >( "Output" ).getRenderTarget( data );
   
   if( !data[ m_normalsAndDepthBuffer ] )
   {
      return;
   }

   ResourcesManager& rm = ResourcesManager::getInstance();
   Renderer& renderer = host.getRenderer();
   data[ m_renderer ] = &renderer;

   data[ m_ssaoPass ] = DynamicCast< PixelShader >( &rm.create( SHADERS_DIR "RenderingPipeline/SSAO.tpsh" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPSSAONode::onDeinitialize( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   data[ m_ssaoPass ] = NULL;
   data[ m_outputTarget  ] = NULL;
   data[ m_normalsAndDepthBuffer ] = NULL;
   data[ m_renderer ] = NULL;

   __super::onDeinitialize( host );
}

///////////////////////////////////////////////////////////////////////////////

void RPSSAONode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   Renderer& renderer = *data[ m_renderer ];
   PixelShader* ssaoPass = data[ m_ssaoPass ];
   ShaderTexture* normalsAndDepthBuffer = data[ m_normalsAndDepthBuffer ];
   RenderTarget* outputTarget = data[ m_outputTarget ];

   if ( !normalsAndDepthBuffer || !m_samplingPatternNoise )
   {
      return;
   }

   Camera& activeCamera = renderer.getActiveCamera();
   D3DXVECTOR4 params( (float)normalsAndDepthBuffer->getWidth(), (float)normalsAndDepthBuffer->getHeight(), activeCamera.getFarClippingPlane(), m_occlusionRadius );

   D3DXMATRIX projMtx = activeCamera.getProjectionMtx();
   D3DXMATRIX invProjMtx, transposedInvProjMtx;
   D3DXMatrixInverse( &invProjMtx, NULL, &projMtx );
   D3DXMatrixTranspose( &transposedInvProjMtx, &invProjMtx );

   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *ssaoPass );
   comm->setTexture( "g_NormalsAndDepthBuffer", *normalsAndDepthBuffer );
   comm->setTexture( "g_SamplingPatternNoise", *m_samplingPatternNoise );
   comm->setVec4( "g_Params", params );
   comm->setMtx( "g_InvProj", transposedInvProjMtx );

   // render
   renderQuad( renderer, outputTarget );
   new ( renderer() ) RCUnbindPixelShader( *ssaoPass );

}

///////////////////////////////////////////////////////////////////////////////
