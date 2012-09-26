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
#include "core-Renderer\RenderTargetDescriptor.h"
#include "core-Renderer\DepthBufferDescriptor.h"
#include "core-Renderer\RenderingPipelineTransaction.h"


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
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::RPDeferredLightingNode()
{
   m_normalsInput = new RPTextureInput( "Normals" );
   m_specularInput = new RPTextureInput( "Specular" );
   m_depthInput = new RPTextureInput( "Depth" );
   m_sceneColorInput = new RPTextureInput( "SceneColor" );
   m_materialIndicesInput = new RPTextureInput( "MaterialsIndices" );
   m_materialsDescInput = new RPTextureInput( "MaterialsDescr" );

   m_finalLightColorTargetOutput = new RPTextureOutput( "LitScene" );
   m_shadowDepthTextureOutput = new RPTextureOutput( "ShadowDepthBuffer" );
   m_screenSpaceShadowMapOutput = new RPTextureOutput( "SS_ShadowMap" );

   std::vector< GBNodeInput< RenderingPipelineNode >* > inputs;
   inputs.push_back( m_normalsInput );
   inputs.push_back( m_specularInput );
   inputs.push_back( m_depthInput );
   inputs.push_back( m_sceneColorInput );
   inputs.push_back( m_materialIndicesInput );
   inputs.push_back( m_materialsDescInput );
   defineInputs( inputs );

   std::vector< GBNodeOutput< RenderingPipelineNode >* > outputs;
   outputs.push_back( m_finalLightColorTargetOutput );
   outputs.push_back( m_shadowDepthTextureOutput );
   outputs.push_back( m_screenSpaceShadowMapOutput );
   defineOutputs( outputs );
}

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::~RPDeferredLightingNode()
{
   m_normalsInput = NULL;
   m_specularInput = NULL;
   m_depthInput = NULL;
   m_sceneColorInput = NULL;
   m_materialIndicesInput = NULL;
   m_materialsDescInput = NULL;

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
   delete m_materialIndicesInput;
   delete m_materialsDescInput;

   delete m_finalLightColorTargetOutput;
   delete m_shadowDepthTextureOutput;
   delete m_screenSpaceShadowMapOutput;

   m_normalsInput = static_cast< RPTextureInput* >( findInput( "Normals" ) );
   m_specularInput = static_cast< RPTextureInput* >( findInput( "Specular" ) );
   m_depthInput = static_cast< RPTextureInput* >( findInput( "Depth" ) );
   m_sceneColorInput = static_cast< RPTextureInput* >( findInput( "SceneColor" ) );
   m_materialIndicesInput = static_cast< RPTextureInput* >( findInput( "MaterialsIndices" ) );
   m_materialsDescInput = static_cast< RPTextureInput* >( findInput( "MaterialsDescr" ) );

   m_finalLightColorTargetOutput = static_cast< RPTextureOutput* >( findOutput( "LitScene"  ) );
   m_shadowDepthTextureOutput = static_cast< RPTextureOutput* >( findOutput( "ShadowDepthBuffer" ) );
   m_screenSpaceShadowMapOutput = static_cast< RPTextureOutput* >( findOutput( "SS_ShadowMap" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onCreatePrerequisites( RenderingPipelineTransaction& transaction ) const
{
   // create necessary render targets and depth buffers

   // material descriptor render target
   {
      RenderTargetDescriptor* rtDesc = new RenderTargetDescriptor();
      rtDesc->setTargetID( "SS_ShadowMap" );
      rtDesc->setDynamicSize( 1.0f, 1.0f );
      rtDesc->setType( true );
      rtDesc->setUsage( TU_32BIT_FP );

      transaction.addRenderTarget( rtDesc );
   }

   // texture atlas render target
   {
      RenderTargetDescriptor* rtDesc = new RenderTargetDescriptor();
      rtDesc->setTargetID( "ShadowDepthBuffer" );
      rtDesc->setStaticSize( 2048, 2048 );
      rtDesc->setType( false );
      rtDesc->setUsage( TU_32BIT_FP );

      transaction.addRenderTarget( rtDesc );
   }

   // texture atlas depth buffer
   {
      DepthBufferDescriptor* dbDesc = new DepthBufferDescriptor();
      dbDesc->setID( "ShadowDepthBuffer" );
      dbDesc->setSize( 2048, 2048 );

      transaction.addDepthBuffer( dbDesc );
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
      RenderTarget* shadowDepthTexture = host.getRenderTarget( "ShadowDepthBuffer" );
      data[ m_shadowDepthTexture ] = shadowDepthTexture;

      DepthBuffer* shadowDepthSurface = host.getDepthBuffer( "ShadowDepthBuffer" );
      data[ m_shadowDepthSurface ] = shadowDepthSurface;

      RenderTarget* screenSpaceShadowMap = host.getRenderTarget( "SS_ShadowMap" );
      data[ m_screenSpaceShadowMap ] = screenSpaceShadowMap;

      RenderTarget* finalLightColorTarget = host.getRenderTarget( "LitScene" );
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

   if ( m_materialIndicesInput )
   {
      renderingData.m_materialIndicesTex = m_materialIndicesInput->getValue( data );
   }

   if ( m_materialsDescInput )
   {
      renderingData.m_materialsDescriptorsTex = m_materialsDescInput->getValue( data );
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
      ambientLightRenderer->render( renderer, ambientLight, renderingData );
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
