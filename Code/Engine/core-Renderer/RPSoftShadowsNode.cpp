#include "core-Renderer/RPSoftShadowsNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/FullscreenQuad.h"
#include "core-Renderer/Light.h"
#include "core-Renderer/MRTUtil.h"
#include "core/MemoryPool.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSoftShadowsNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Shadow depth texture id", std::string, m_shadowDepthTextureId );
   PROPERTY_EDIT( "Shadow depth surface id", std::string, m_shadowDepthSurfaceId );
   PROPERTY_EDIT( "Shadow map id", std::string, m_screenSpaceShadowMapId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPSoftShadowsNode::RPSoftShadowsNode()
   : m_shadowDepthTextureId( "ShadowDepthTexture" )
   , m_shadowDepthSurfaceId( "ShadowDepthSurface" )
   , m_screenSpaceShadowMapId( "SS_ShadowMap" )
{
   defineInput( new RPVoidInput( "Input" ) );

   m_shadowDepthTextureOutput = new RPTextureOutput( m_shadowDepthTextureId );
   defineOutput( m_shadowDepthTextureOutput );

   m_screenSpaceShadowMapOutput = new RPTextureOutput( m_screenSpaceShadowMapId );
   defineOutput( m_screenSpaceShadowMapOutput );
}
///////////////////////////////////////////////////////////////////////////////

RPSoftShadowsNode::~RPSoftShadowsNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPSoftShadowsNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find output sockets
   delete m_shadowDepthTextureOutput;
   m_shadowDepthTextureOutput = DynamicCast< RPTextureOutput >( findOutput( m_shadowDepthTextureId ) );

   delete m_screenSpaceShadowMapOutput;
   m_screenSpaceShadowMapOutput = DynamicCast< RPTextureOutput >( findOutput( m_screenSpaceShadowMapId ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPSoftShadowsNode::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_shadowDepthTextureId" || property.getName() == "m_screenSpaceShadowMapId" )
   {
      std::string rtIds;
      rtIds = m_shadowDepthTextureId + ";" + m_screenSpaceShadowMapId;
      MRTUtil::defineOutputs( rtIds, this );

      m_shadowDepthTextureOutput = DynamicCast< RPTextureOutput >( findOutput( m_shadowDepthTextureId ) );
      m_screenSpaceShadowMapOutput = DynamicCast< RPTextureOutput >( findOutput( m_screenSpaceShadowMapId ) );

   } 
}

///////////////////////////////////////////////////////////////////////////////

void RPSoftShadowsNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_shadowDepthTexture );
   data.registerVar( m_shadowDepthSurface );
   data.registerVar( m_screenSpaceShadowMap );
   data.registerVar( m_treeMemPool );

   // create render targets
   {
      RenderTarget* shadowDepthTexture = host.getRenderTarget( m_shadowDepthTextureId );
      data[ m_shadowDepthTexture ] = shadowDepthTexture;

      DepthBuffer* shadowDepthSurface = host.getDepthBuffer( m_shadowDepthSurfaceId );
      data[ m_shadowDepthSurface ] = shadowDepthSurface;

      RenderTarget* screenSpaceShadowMap = host.getRenderTarget( m_screenSpaceShadowMapId );
      data[ m_screenSpaceShadowMap ] = screenSpaceShadowMap;

   }

   // create a memory pool for the render tree
   {
      MemoryPool* treeMemPool = new MemoryPool( 1024 * 1024 );
      data[ m_treeMemPool ] = treeMemPool;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSoftShadowsNode::onDestroyLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   MemoryPool* treeMemPool = data[ m_treeMemPool ];
   delete treeMemPool;
}

///////////////////////////////////////////////////////////////////////////////

void RPSoftShadowsNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();
   Renderer& renderer = host.getRenderer();


   // get the render targets used to render the shadow
   RenderTarget* shadowDepthTexture = data[ m_shadowDepthTexture ];
   DepthBuffer* shadowDepthSurface = data[ m_shadowDepthSurface ];
   RenderTarget* screenSpaceShadowMap = data[ m_screenSpaceShadowMap ];

   if ( shadowDepthTexture && shadowDepthSurface )
   {
      // The shadow depth buffer needs to be an off-screen render target. The other one can be the back buffer
    
      // render the scene as many times as there are visible lights that cast shadows
      ShadowRendererData data;
      data.m_renderingView = host.getSceneRenderingView();
      data.m_geometryToRender = &host.getSceneElements();
      data.m_shadowDepthTexture = shadowDepthTexture;
      data.m_shadowDepthSurface = shadowDepthSurface;
      data.m_screenSpaceShadowMap = screenSpaceShadowMap;

      const Array< Light* >& visibleLights = host.getSceneLights();  
      uint lightsCount = visibleLights.size();
      for ( uint i = 0; i < lightsCount; ++i )
      {
         Light* light = visibleLights[i];

         light->renderShadowMap( renderer, data );
      }
   }

   // send the final shadow map to the output
   m_shadowDepthTextureOutput->setValue( data, shadowDepthTexture );
   m_screenSpaceShadowMapOutput->setValue( data, screenSpaceShadowMap );
}

///////////////////////////////////////////////////////////////////////////////
