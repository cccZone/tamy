#include "core-Renderer/RPLightIndicesNode.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingView.h"
#include "core-Renderer/MRTUtil.h"

// lights
#include "core-Renderer/Light.h"
#include "core-Renderer/AmbientLight.h"
#include "core-Renderer/DirectionalLight.h"
#include "core-Renderer/PointLight.h"

// rendering methods
#include "core-Renderer/IndexedLightingRenderData.h"
#include "core-Renderer/IndexedDirectionalLightRenderer.h"
//#include "core-Renderer/IndexedPointLightRenderer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPLightIndicesNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Lights color buffer id", std::string, m_lightsColorTargetId );
   PROPERTY_EDIT( "Lights direction buffer id", std::string, m_lightsDirectionTargetId );
   PROPERTY_EDIT( "Shadow depth buffer id", std::string, m_shadowDepthBufferId );
   PROPERTY_EDIT( "Shadow depth surface id", std::string, m_shadowDepthSurfaceId );
   PROPERTY_EDIT( "Shadow map id", std::string, m_screenSpaceShadowMapId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPLightIndicesNode::RPLightIndicesNode()
   : m_lightsColorTargetId( "LightsColor" )
   , m_lightsDirectionTargetId( "LightsDirection" )
   , m_shadowDepthBufferId( "ShadowDepthbuffer" )
   , m_shadowDepthSurfaceId( "ShadowDepthSurface" )
   , m_screenSpaceShadowMapId( "SS_ShadowMap" )
{
   m_normalsInput = new RPTextureInput( "Normals" );
   m_depthInput = new RPTextureInput( "Depth" );

   std::vector< GBNodeInput< RenderingPipelineNode >* > inputs;
   inputs.push_back( m_normalsInput );
   inputs.push_back( m_depthInput );
   defineInputs( inputs );

   bool isBeingDeserialized = SerializationFlag::getInstance().isSerializationInProgress();
   if ( !isBeingDeserialized )
   {
      std::string allRenderTargetIds = m_lightsColorTargetId + ";" + m_lightsDirectionTargetId + ";" + m_shadowDepthBufferId + ";" + m_screenSpaceShadowMapId;
      std::vector< GBNodeOutput< RenderingPipelineNode >* > outputs;
      MRTUtil::createOutputs( allRenderTargetIds, outputs );
      defineOutputs( outputs );

      m_lightsColorOutput = static_cast< RPTextureOutput* >( findOutput( m_lightsColorTargetId ) );
      m_lightsDirectionOutput = static_cast< RPTextureOutput* >( findOutput( m_lightsDirectionTargetId ) );
      m_shadowDepthBufferOutput = static_cast< RPTextureOutput* >( findOutput( m_shadowDepthBufferId ) );
      m_screenSpaceShadowMapOutput = static_cast< RPTextureOutput* >( findOutput( m_screenSpaceShadowMapId ) );
   }
   else
   {
      m_lightsColorOutput = NULL;
      m_lightsDirectionOutput = NULL;
      m_shadowDepthBufferOutput = NULL;
      m_screenSpaceShadowMapOutput = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

RPLightIndicesNode::~RPLightIndicesNode()
{
   m_normalsInput = NULL;
   m_depthInput = NULL;

   m_lightsColorOutput = NULL;
   m_lightsDirectionOutput = NULL;
   m_shadowDepthBufferOutput = NULL;
   m_screenSpaceShadowMapOutput = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPLightIndicesNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find input socket
   delete m_normalsInput;
   delete m_depthInput;
   delete m_lightsColorOutput;
   delete m_lightsDirectionOutput;
   delete m_shadowDepthBufferOutput;
   delete m_screenSpaceShadowMapOutput;

   m_normalsInput = DynamicCast< RPTextureInput >( findInput( "Normals" ) );
   m_depthInput = DynamicCast< RPTextureInput >( findInput( "Depth" ) );

   m_lightsColorOutput = static_cast< RPTextureOutput* >( findOutput( m_lightsColorTargetId ) );
   m_lightsDirectionOutput = static_cast< RPTextureOutput* >( findOutput( m_lightsDirectionTargetId ) );
   m_shadowDepthBufferOutput = static_cast< RPTextureOutput* >( findOutput( m_shadowDepthBufferId ) );
   m_screenSpaceShadowMapOutput = static_cast< RPTextureOutput* >( findOutput( m_screenSpaceShadowMapId ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPLightIndicesNode::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   const std::string& propertyName = property.getName();
   if ( propertyName == "m_lightsColorTargetId" || propertyName == "m_lightsDirectionTargetId" || propertyName == "m_shadowDepthBufferId" || propertyName == "m_screenSpaceShadowMapId" )
   {
      std::string allRenderTargetIds = m_lightsColorTargetId + ";" + m_lightsDirectionTargetId + ";" + m_shadowDepthBufferId + ";" + m_screenSpaceShadowMapId;
      std::vector< GBNodeOutput< RenderingPipelineNode >* > outputs;
      MRTUtil::createOutputs( allRenderTargetIds, outputs );
      redefineOutputs( outputs );

      // acquire new output instances
      m_lightsColorOutput = static_cast< RPTextureOutput* >( findOutput( m_lightsColorTargetId ) );
      m_lightsDirectionOutput = static_cast< RPTextureOutput* >( findOutput( m_lightsDirectionTargetId ) );
      m_shadowDepthBufferOutput = static_cast< RPTextureOutput* >( findOutput( m_shadowDepthBufferId ) );
      m_screenSpaceShadowMapOutput = static_cast< RPTextureOutput* >( findOutput( m_screenSpaceShadowMapId ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPLightIndicesNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_lightsColorTarget );
   data.registerVar( m_lightsDirectionTarget );
   data.registerVar( m_shadowDepthBuffer );
   data.registerVar( m_shadowDepthSurface );
   data.registerVar( m_screenSpaceShadowMap );
   data.registerVar( m_directionalLightsRenderer );

   // create render targets
   {
      RenderTarget* lightsColorTarget = host.getRenderTarget( m_lightsColorTargetId );
      data[ m_lightsColorTarget ] = lightsColorTarget;

      RenderTarget* lightsDirectionTarget = host.getRenderTarget( m_lightsDirectionTargetId );
      data[ m_lightsDirectionTarget ] = lightsDirectionTarget;

      RenderTarget* shadowDeptBuffer = host.getRenderTarget( m_shadowDepthBufferId );
      data[ m_shadowDepthBuffer ] = shadowDeptBuffer;

      DepthBuffer* shadowDepthSurface = host.getDepthBuffer( m_shadowDepthSurfaceId );
      data[ m_shadowDepthSurface ] = shadowDepthSurface;

      RenderTarget* screenSpaceShadowMap = host.getRenderTarget( m_screenSpaceShadowMapId );
      data[ m_screenSpaceShadowMap ] = screenSpaceShadowMap;
   }

   // create dedicated light renderers
   {
      data[ m_directionalLightsRenderer ] = new IndexedDirectionalLightRenderer( 4, 2 );
      //data[ m_pointLightsRenderer ] = new AccumulatedPointLightRenderer();
   }

}

///////////////////////////////////////////////////////////////////////////////

void RPLightIndicesNode::onDestroyLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   delete data[ m_directionalLightsRenderer ];
   data[ m_directionalLightsRenderer ] = NULL;
   /*
   delete data[ m_pointLightsRenderer ];
   data[ m_pointLightsRenderer ] = NULL;
   */
}

///////////////////////////////////////////////////////////////////////////////

void RPLightIndicesNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   IndexedDirectionalLightRenderer* directionalLightsRenderer = data[ m_directionalLightsRenderer ];
   if ( !directionalLightsRenderer )
   {
      // no renderers on board - no rendering can take place
      return;
   }


   IndexedLightingRenderData renderingData;
   memset( &renderingData, 0, sizeof( IndexedLightingRenderData ) );

   if ( m_depthInput )
   {
      renderingData.m_depthTex = m_depthInput->getValue( data );
   }

   if ( m_normalsInput )
   {
      renderingData.m_normalsTex = m_normalsInput->getValue( data );
   }

   Renderer& renderer = host.getRenderer();

   const RenderingView* view = host.getSceneRenderingView();
   renderingData.m_renderingView = view;
   renderingData.m_geometryToRender = &host.getSceneElements();
   renderingData.m_shadowDepthTexture = data[ m_shadowDepthBuffer ];
   renderingData.m_shadowDepthSurface = data[ m_shadowDepthSurface ];
   renderingData.m_screenSpaceShadowMap = data[ m_screenSpaceShadowMap ];
   //renderingData.m_lightsDirectionsTarget = data[ m_lightsDirectionTarget ];
   //renderingData.m_lightsColorsTarget = data[ m_lightsColorTarget ];


   // we don't index the ambient light - lighting calculations are just gonna take it into account

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
            directionalLightsRenderer->render( renderer, static_cast< DirectionalLight* >( light ), renderingData, i );

            break;
         }

      case Light::LT_Point:
         {
            //pointLightsRenderer->render( renderer, static_cast< PointLight* >( light ), renderingData );
            break;
         }
      }

   }
   // set the outputs
   m_shadowDepthBufferOutput->setValue( data, renderingData.m_shadowDepthTexture );
   m_screenSpaceShadowMapOutput->setValue( data, renderingData.m_screenSpaceShadowMap );
   m_lightsDirectionOutput->setValue( data, renderingData.m_lightIndicesTarget );
}

///////////////////////////////////////////////////////////////////////////
