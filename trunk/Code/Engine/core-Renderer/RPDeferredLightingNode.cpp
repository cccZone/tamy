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
   PROPERTY_EDIT( "Render target id", std::string, m_renderTargetId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::RPDeferredLightingNode()
   : m_shader( NULL )
{
   m_depthNormalBufferInput = new RPTextureInput( "DepthNormals" );
   m_lightTexture = new RPTextureOutput( "LightsTexture" );

   defineInput( m_depthNormalBufferInput );
   defineOutput( m_lightTexture );
}

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::~RPDeferredLightingNode()
{
   m_depthNormalBufferInput = NULL;
   m_lightTexture = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // load the lighting shader
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   FilePath shaderPath( "/Renderer/Shaders/RenderingPipeline/DeferredLighting.tpsh" );
   m_shader = resMgr.create< PixelShader >( shaderPath );

   // initialize node sockets
   initializeSocketd();
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::initializeSocketd()
{
   m_depthNormalBufferInput = DynamicCast< RPTextureInput >( findInput( "DepthNormals" ) );
   m_lightTexture = DynamicCast< RPTextureOutput >( findOutput( "LightsTexture" ) );

}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_renderTarget );

   // create the render target
   RenderTarget* trg = NULL;
   {
      trg = host.getRenderTarget( m_renderTargetId );
      data[ m_renderTarget ] = trg;
   }

   // find the existing outputs and set the data on them
   {
      RPTextureOutput* output = DynamicCast< RPTextureOutput >( findOutput( "LightsTexture" ) );
      output->setValue( data, trg );
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
   if ( !m_shader || !depthNormalsBufferTex )
   {
      // no scene - no rendering
      return;
   }

   RenderTarget* trg = data[ m_renderTarget ];
   Renderer& renderer = host.getRenderer();

   // activate the render target we want to render to
   new ( renderer() ) RCActivateRenderTarget( trg );

   // set the shader data
   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader );
   {
      comm->setTexture( "depthNormalsTex", *depthNormalsBufferTex );
   }

   // render light volumes
   const Array< Light* >& visibleLights = host.getSceneLights();  
   uint lightsCount = visibleLights.size();
   for ( uint i = 0; i < lightsCount; ++i )
   {
      Light* light = visibleLights[i];
      light->render( renderer );
   }

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_shader );
   new ( renderer() ) RCDeactivateRenderTarget();
}

///////////////////////////////////////////////////////////////////////////////
