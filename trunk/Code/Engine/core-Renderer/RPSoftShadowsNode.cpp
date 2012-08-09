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
   PROPERTY_EDIT( "Shadow depth buf id", std::string, m_shadowDepthBufferId );
   PROPERTY_EDIT( "Shadow map id", std::string, m_screenSpaceShadowMapId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPSoftShadowsNode::RPSoftShadowsNode()
   : m_shadowDepthBufferId( "ShadowDepthBuffer" )
   , m_screenSpaceShadowMapId( "SS_ShadowMap" )
{
   defineInput( new RPVoidInput( "Input" ) );

   m_shadowDepthBufferOutput = new RPTextureOutput( m_shadowDepthBufferId );
   defineOutput( m_shadowDepthBufferOutput );

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
   delete m_shadowDepthBufferOutput;
   m_shadowDepthBufferOutput = DynamicCast< RPTextureOutput >( findOutput( m_shadowDepthBufferId ) );

   delete m_screenSpaceShadowMapOutput;
   m_screenSpaceShadowMapOutput = DynamicCast< RPTextureOutput >( findOutput( m_screenSpaceShadowMapId ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPSoftShadowsNode::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_shadowDepthBufferId" || property.getName() == "m_screenSpaceShadowMapId" )
   {
      std::string rtIds;
      rtIds = m_shadowDepthBufferId + ";" + m_screenSpaceShadowMapId;
      MRTUtil::defineOutputs( rtIds, this );

      m_shadowDepthBufferOutput = DynamicCast< RPTextureOutput >( findOutput( m_shadowDepthBufferId ) );
      m_screenSpaceShadowMapOutput = DynamicCast< RPTextureOutput >( findOutput( m_screenSpaceShadowMapId ) );

   } 
}

///////////////////////////////////////////////////////////////////////////////

void RPSoftShadowsNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_shadowDepthBuffer );
   data.registerVar( m_screenSpaceShadowMap );
   data.registerVar( m_treeMemPool );

   // create render targets
   {
      RenderTarget* shadowDepthBuffer = host.getRenderTarget( m_shadowDepthBufferId );
      data[ m_shadowDepthBuffer ] = shadowDepthBuffer;

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
   RenderTarget* shadowDepthBuffer = data[ m_shadowDepthBuffer];
   RenderTarget* screenSpaceShadowMap = data[ m_screenSpaceShadowMap ];

   if ( shadowDepthBuffer )
   {
      // The shadow depth buffer needs to be an off-screen render target. The other one can be the back buffer
    
      // render the scene as many times as there are visible lights that cast shadows
      const RenderingView* renderingView = host.getSceneRenderingView();

      const Array< Light* >& visibleLights = host.getSceneLights();  
      uint lightsCount = visibleLights.size();
      for ( uint i = 0; i < lightsCount; ++i )
      {
         Light* light = visibleLights[i];

         light->renderShadowMap( renderer, shadowDepthBuffer, screenSpaceShadowMap, renderingView );
      }
   }

   // send the final shadow map to the output
   m_shadowDepthBufferOutput->setValue( data, shadowDepthBuffer );
   m_screenSpaceShadowMapOutput->setValue( data, screenSpaceShadowMap );
}

///////////////////////////////////////////////////////////////////////////////
