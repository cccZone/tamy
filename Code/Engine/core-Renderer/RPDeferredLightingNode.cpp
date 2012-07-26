#include "core-Renderer/RPDeferredLightingNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/Light.h"
#include "core/ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPDeferredLightingNode );
   PARENT( RenderingPipelineNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::RPDeferredLightingNode()
{
   m_depthNormalBufferInput = new RPTextureInput( "DepthNormals" );
   m_lightsDirection = new RPTextureOutput( "LightsDirection" );
   m_lightsColor = new RPTextureOutput( "LightsColor" );

   defineInput( m_depthNormalBufferInput );
   defineOutput( m_lightsDirection );
   defineOutput( m_lightsColor );
}

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::~RPDeferredLightingNode()
{
   m_depthNormalBufferInput = NULL;
   m_lightsDirection = NULL;
   m_lightsColor = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // initialize node sockets
   initializeSocketd();
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::initializeSocketd()
{
   m_depthNormalBufferInput = DynamicCast< RPTextureInput >( findInput( "DepthNormals" ) );
   m_lightsDirection = DynamicCast< RPTextureOutput >( findOutput( "LightsDirection" ) );
   m_lightsColor = DynamicCast< RPTextureOutput >( findOutput( "LightsColor" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_lightsDirectionRT );
   data.registerVar( m_lightsColorRT );

   // create the render targets
   RenderTarget* lightsDirectionRT = NULL;
   RenderTarget* lightsColorRT = NULL;
   {
      lightsDirectionRT = host.getRenderTarget( "LightsDirection" );
      lightsColorRT = host.getRenderTarget( "LightsColor" ); 
      data[ m_lightsDirectionRT ] = lightsDirectionRT;
      data[ m_lightsColorRT ] = lightsColorRT;
   }

   // find the existing outputs and set the data on them
   {
      RPTextureOutput* lightsDirectionOutput = DynamicCast< RPTextureOutput >( findOutput( "LightsDirection" ) );
      lightsDirectionOutput->setValue( data, lightsDirectionRT );

      RPTextureOutput* lightsColorOutput = DynamicCast< RPTextureOutput >( findOutput( "LightsColor" ) );
      lightsColorOutput->setValue( data, lightsColorRT );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onDestroyLayout( RenderingPipelineMechanism& host ) const
{
   // nothing to release here
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !m_depthNormalBufferInput )
   {
      return;
   }
   RuntimeDataBuffer& data = host.data();

   ShaderTexture* depthNormalsBufferTex = m_depthNormalBufferInput->getValue( data );
   RenderTarget* lightsDirectionRT = data[ m_lightsDirectionRT ];
   RenderTarget* lightsColorRT = data[ m_lightsColorRT ];

   if ( !depthNormalsBufferTex || !lightsDirectionRT || !lightsColorRT )
   {
      // no scene - no rendering
      return;
   }

   Renderer& renderer = host.getRenderer();

   // activate the render target we want to render to
   new ( renderer() ) RCActivateRenderTarget( lightsDirectionRT, 0 );
   new ( renderer() ) RCActivateRenderTarget( lightsColorRT, 1 );

   // render light volumes
   const Array< Light* >& visibleLights = host.getSceneLights();  
   uint lightsCount = visibleLights.size();
   for ( uint i = 0; i < lightsCount; ++i )
   {
      Light* light = visibleLights[i];
      light->render( renderer, depthNormalsBufferTex );
   }

   // cleanup
   new ( renderer() ) RCDeactivateRenderTarget( 0 );
   new ( renderer() ) RCDeactivateRenderTarget( 1 );
}

///////////////////////////////////////////////////////////////////////////////
