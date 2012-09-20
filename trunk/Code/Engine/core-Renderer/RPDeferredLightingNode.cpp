#include "core-Renderer/RPDeferredLightingNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/MRTUtil.h"
#include "core/ResourcesManager.h"
#include "core-Renderer/Defines.h"

// lights
#include "core-Renderer/Light.h"
#include "core-Renderer/AmbientLight.h"
#include "core-Renderer/DirectionalLight.h"
#include "core-Renderer/PointLight.h"

// rendering methods
#include "core-Renderer/DeferredLightingRenderData.h"
#include "core-Renderer/DeferredDirectionalLightRenderer.h"
#include "core-Renderer/DeferredPointLightRenderer.h"
#include "core-Renderer/DeferredAmbientLightRenderer.h"


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
   m_normalsInput = new RPTextureInput( "Normals" );
   defineInput( m_normalsInput );

   m_specularInput = new RPTextureInput( "Specular" );
   defineInput( m_specularInput );

   m_depthInput = new RPTextureInput( "Depth" );
   defineInput( m_depthInput );
   
   m_sceneColorInput = new RPTextureInput( "SceneColor" );
   defineInput( m_sceneColorInput );

   m_finalLightColorTargetOutput = new RPTextureOutput( m_finalLightColorTargetId );
   defineOutput( m_finalLightColorTargetOutput );

   m_shadowDepthTextureOutput = new RPTextureOutput( m_shadowDepthTextureId );
   defineOutput( m_shadowDepthTextureOutput );

   m_screenSpaceShadowMapOutput = new RPTextureOutput( m_screenSpaceShadowMapId );
   defineOutput( m_screenSpaceShadowMapOutput );
}

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::~RPDeferredLightingNode()
{
   m_normalsInput = NULL;
   m_specularInput = NULL;
   m_depthInput = NULL;
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
   delete m_normalsInput;
   delete m_specularInput;
   delete m_depthInput;
   delete m_sceneColorInput;
   delete m_finalLightColorTargetOutput;
   delete m_shadowDepthTextureOutput;
   delete m_screenSpaceShadowMapOutput;

   m_normalsInput = DynamicCast< RPTextureInput >( findInput( "Normals" ) );
   m_specularInput = DynamicCast< RPTextureInput >( findInput( "Specular" ) );
   m_depthInput = DynamicCast< RPTextureInput >( findInput( "Depth" ) );
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
   data.registerVar( m_directionalLightsRenderer );
   data.registerVar( m_pointLightsRenderer );
   data.registerVar( m_ambientLightRenderer );

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

   // create dedicated light renderers
   {
      data[ m_directionalLightsRenderer ] = new DeferredDirectionalLightRenderer( 4, 2 );
      data[ m_pointLightsRenderer ] = new DeferredPointLightRenderer();
      data[ m_ambientLightRenderer ] = new DeferredAmbientLightRenderer();
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onDestroyLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   delete data[ m_directionalLightsRenderer ];
   data[ m_directionalLightsRenderer ] = NULL;

   delete data[ m_pointLightsRenderer ];
   data[ m_pointLightsRenderer ] = NULL;

   delete data[ m_ambientLightRenderer ];
   data[ m_ambientLightRenderer ] = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   DeferredDirectionalLightRenderer* directionalLightsRenderer = data[ m_directionalLightsRenderer ];
   DeferredPointLightRenderer* pointLightsRenderer = data[ m_pointLightsRenderer ];
   DeferredAmbientLightRenderer* ambientLightRenderer = data[ m_ambientLightRenderer ];

   if ( !pointLightsRenderer || !directionalLightsRenderer || !ambientLightRenderer )
   {
      // no renderers on board - no rendering can take place
      return;
   }
  
   DeferredLightingRenderData renderingData;
   memset( &renderingData, 0, sizeof( DeferredLightingRenderData ) );

   if ( m_depthInput )
   {
      renderingData.m_depthTex = m_depthInput->getValue( data );
   }

   if ( m_normalsInput )
   {
      renderingData.m_normalsTex = m_normalsInput->getValue( data );
   }

   if ( m_specularInput )
   {
      renderingData.m_specularTex = m_specularInput->getValue( data );
   }

   if ( m_sceneColorInput )
   {
      renderingData.m_sceneColorTex = m_sceneColorInput->getValue( data );
   }

   Renderer& renderer = host.getRenderer();

   const RenderingView* view = host.getSceneRenderingView();
   renderingData.m_renderingView = view;
   renderingData.m_geometryToRender = &host.getSceneElements();
   renderingData.m_shadowDepthTexture = data[ m_shadowDepthTexture ];
   renderingData.m_shadowDepthSurface = data[ m_shadowDepthSurface ];
   renderingData.m_screenSpaceShadowMap = data[ m_screenSpaceShadowMap ];
   renderingData.m_finalLightColorTarget = data[ m_finalLightColorTarget ];


   // render the ambient light first
   AmbientLight* ambientLight = view->getAmbientLight();
   if ( ambientLight )
   {
      ambientLightRenderer->render( renderer, ambientLight, renderingData.m_sceneColorTex, renderingData.m_finalLightColorTarget );
   }

   // collect visible lights - query the host for them rather than the view, 
   // because it pools the visible lights every frame so that all lighting node can operate on the exact same lights
   // and in order to save CPU time needed to query for the visible lights
   const Array< Light* >& visibleLights = host.getSceneLights();  

   // render light volumes - use the additive color blend to blend on top of the base color
   // prepared by the ambient light
   uint lightsCount = visibleLights.size();
   for ( uint i = 0; i < lightsCount; ++i )
   {
      Light* light = visibleLights[i];
      switch( light->getType() )
      {
      case Light::LT_Directional:
         {
            directionalLightsRenderer->render( renderer, static_cast< DirectionalLight* >( light ), renderingData );

            break;
         }

      case Light::LT_Point:
         {
            pointLightsRenderer->render( renderer, static_cast< PointLight* >( light ), renderingData );
            break;
         }
      }
   }

   // set the outputs
   m_shadowDepthTextureOutput->setValue( data, renderingData.m_shadowDepthTexture );
   m_screenSpaceShadowMapOutput->setValue( data, renderingData.m_screenSpaceShadowMap );
   m_finalLightColorTargetOutput->setValue( data, renderingData.m_finalLightColorTarget );
}

///////////////////////////////////////////////////////////////////////////////
