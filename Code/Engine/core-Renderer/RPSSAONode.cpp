#include "core-Renderer/RPSSAONode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/TextureSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/Defines.h"
#include "core-Renderer/Camera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSSAONode, RPPostProcessNode );
   PROPERTY_EDIT( "Radius", float, m_occlusionRadius );
   PROPERTY_EDIT( "Min Dist", float, m_fullOcclusionThreshold );
   PROPERTY_EDIT( "Max Dist", float, m_noOcclusionThreshold );
   PROPERTY_EDIT( "Power", float, m_occlusionPower );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPSSAONode::RPSSAONode()
{
   defineInput( new RPTextureInput( "InputTex" ) );
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

   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   Renderer& renderer = host.getRenderer();
   data[ m_renderer ] = &renderer;

   data[ m_ssaoPass ] = new PixelShader( SHADERS_DIR "RenderingPipeline/SSAO.tpsh" );
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

   if ( !normalsAndDepthBuffer )
   {
      return;
   }

   Camera& activeCamera = renderer.getActiveCamera();
   D3DXVECTOR4 planeSizes( normalsAndDepthBuffer->getWidth(), normalsAndDepthBuffer->getHeight(), activeCamera.getNearPlaneWidth(), activeCamera.getNearPlaneHeight() );

   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *ssaoPass );
   comm->setTexture( "g_NormalsAndDepthBuffer", *normalsAndDepthBuffer );
   comm->setVec4( "g_PlaneSizes", planeSizes );
   comm->setFloat( "g_OcclusionRadius", m_occlusionRadius );
   comm->setFloat( "g_NoOcclusionThreshold", m_noOcclusionThreshold );
   comm->setFloat( "g_FullOcclusionThreshold", m_fullOcclusionThreshold );
   comm->setFloat( "g_OcclusionPower", m_occlusionPower );

   // TODO: set the sampling points 

   // render
   renderQuad( renderer, outputTarget );
   new ( renderer() ) RCUnbindPixelShader( *ssaoPass );

}

///////////////////////////////////////////////////////////////////////////////
