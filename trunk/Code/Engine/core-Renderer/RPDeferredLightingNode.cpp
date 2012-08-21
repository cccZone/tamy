#include "core-Renderer/RPDeferredLightingNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/Light.h"
#include "core-Renderer/MRTUtil.h"
#include "core/ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPDeferredLightingNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Render target ids", std::string, m_finalLightColorTargetId );
   PROPERTY_EDIT( "Shadow depth texture id", std::string, m_shadowDepthTextureId );
   PROPERTY_EDIT( "Shadow depth surface id", std::string, m_shadowDepthSurfaceId );
   PROPERTY_EDIT( "Shadow map id", std::string, m_screenSpaceShadowMapId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::RPDeferredLightingNode()
   : m_finalLightColorTargetId( "LitScene" )
   , m_shadowDepthTextureId( "ShadowDepthTexture" )
   , m_shadowDepthSurfaceId( "ShadowDepthSurface" )
   , m_screenSpaceShadowMapId( "SS_ShadowMap" )
{
   m_depthNormalsInput = new RPTextureInput( "DepthNormals" );
   defineInput( m_depthNormalsInput );
   
   m_sceneColorInput = new RPTextureInput( "SceneColor" );
   defineInput( m_sceneColorInput );

   m_finalLightColorTargetOutput = new RPTextureOutput( m_finalLightColorTargetId );
   defineOutput( m_finalLightColorTargetOutput );

   m_shadowDepthTextureOutput = new RPTextureOutput( m_shadowDepthTextureId );
   defineOutput( m_shadowDepthTextureOutput );

   m_screenSpaceShadowMapOutput = new RPTextureOutput( m_screenSpaceShadowMapId );
   defineOutput( m_screenSpaceShadowMapOutput );


   // <memory.todo> when the object is being loaded, the outputs created here will leak memory. Fix that - maybe by introducing a separate serialization constructor...
   std::string allRenderTargetIds = m_finalLightColorTargetId + ";" + m_shadowDepthTextureId + ";" + m_screenSpaceShadowMapId;
   MRTUtil::defineOutputs( allRenderTargetIds, this );
}

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::~RPDeferredLightingNode()
{
   m_depthNormalsInput = NULL;
   m_sceneColorInput = NULL;

   m_finalLightColorTargetOutput = NULL;
   m_shadowDepthTextureOutput = NULL;
   m_screenSpaceShadowMapOutput = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find input socket
   delete m_depthNormalsInput;
   delete m_sceneColorInput;
   delete m_finalLightColorTargetOutput;
   delete m_shadowDepthTextureOutput;
   delete m_screenSpaceShadowMapOutput;


   m_depthNormalsInput = DynamicCast< RPTextureInput >( findInput( "DepthNormals" ) );
   m_sceneColorInput = DynamicCast< RPTextureInput >( findInput( "SceneColor" ) );

   m_finalLightColorTargetOutput = DynamicCast< RPTextureOutput >( findOutput( m_finalLightColorTargetId ) );
   m_shadowDepthTextureOutput = DynamicCast< RPTextureOutput >( findOutput( m_shadowDepthTextureId ) );
   m_screenSpaceShadowMapOutput = DynamicCast< RPTextureOutput >( findOutput( m_screenSpaceShadowMapId ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   const std::string& propertyName = property.getName();
   if ( propertyName == "m_renderTargetId" || propertyName == "m_shadowDepthTextureId" || propertyName == "m_screenSpaceShadowMapId" )
   {

      std::string allRenderTargetIds = m_finalLightColorTargetId + ";" + m_shadowDepthTextureId + ";" + m_screenSpaceShadowMapId;
      MRTUtil::defineOutputs( allRenderTargetIds, this );

      // acquire new output instances
      m_finalLightColorTargetOutput = DynamicCast< RPTextureOutput >( findOutput( m_finalLightColorTargetId ) );
      m_shadowDepthTextureOutput = DynamicCast< RPTextureOutput >( findOutput( m_shadowDepthTextureId ) );
      m_screenSpaceShadowMapOutput = DynamicCast< RPTextureOutput >( findOutput( m_screenSpaceShadowMapId ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_shadowDepthTexture );
   data.registerVar( m_shadowDepthSurface );
   data.registerVar( m_screenSpaceShadowMap );
   data.registerVar( m_finalLightColorTarget );

   // create render targets
   {
      RenderTarget* shadowDepthTexture = host.getRenderTarget( m_shadowDepthTextureId );
      data[ m_shadowDepthTexture ] = shadowDepthTexture;

      DepthBuffer* shadowDepthSurface = host.getDepthBuffer( m_shadowDepthSurfaceId );
      data[ m_shadowDepthSurface ] = shadowDepthSurface;

      RenderTarget* screenSpaceShadowMap = host.getRenderTarget( m_screenSpaceShadowMapId );
      data[ m_screenSpaceShadowMap ] = screenSpaceShadowMap;

      RenderTarget* finalLightColorTarget = host.getRenderTarget( m_finalLightColorTargetId );
      data[ m_finalLightColorTarget ] = finalLightColorTarget;
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onDestroyLayout( RenderingPipelineMechanism& host ) const
{
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !m_depthNormalsInput || !m_sceneColorInput )
   {
      return;
   }

   RuntimeDataBuffer& data = host.data();

   ShaderTexture* depthNormalsTex = m_depthNormalsInput->getValue( data );
   ShaderTexture* sceneColorTex = m_sceneColorInput->getValue( data );
   if ( !depthNormalsTex || !sceneColorTex )
   {
      // no scene - no rendering
      return;
   }


   Renderer& renderer = host.getRenderer();

   // get the render targets used to render the shadow
   RenderTarget* shadowDepthTexture = data[ m_shadowDepthTexture ];
   DepthBuffer* shadowDepthSurface = data[ m_shadowDepthSurface ];
   RenderTarget* screenSpaceShadowMap = data[ m_screenSpaceShadowMap ];
   RenderTarget* finalLightColorTarget = data[ m_finalLightColorTarget ];


   LightingRenderData renderingData;
   renderingData.m_renderingView = host.getSceneRenderingView();
   renderingData.m_geometryToRender = &host.getSceneElements();
   renderingData.m_shadowDepthTexture = shadowDepthTexture;
   renderingData.m_shadowDepthSurface = shadowDepthSurface;
   renderingData.m_screenSpaceShadowMap = screenSpaceShadowMap;
   renderingData.m_depthNormalsTex = depthNormalsTex;
   renderingData.m_sceneColorTex = sceneColorTex;
   renderingData.m_finalLightColorTarget = finalLightColorTarget;


   // render light volumes
   const Array< Light* >& visibleLights = host.getSceneLights();  
   uint lightsCount = visibleLights.size();
   for ( uint i = 0; i < lightsCount; ++i )
   {
      Light* light = visibleLights[i];

      light->render( renderer, renderingData );
   }

   // set the outputs
   m_shadowDepthTextureOutput->setValue( data, shadowDepthTexture );
   m_screenSpaceShadowMapOutput->setValue( data, screenSpaceShadowMap );
   m_finalLightColorTargetOutput->setValue( data, finalLightColorTarget );
}

///////////////////////////////////////////////////////////////////////////////
