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
   PROPERTY_EDIT( "Rendered scene id", RPMSceneId, m_renderedSceneId );
   PROPERTY_EDIT( "Render target id", std::string, m_renderTargetId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::RPDeferredLightingNode()
   : m_renderedSceneId( RPS_Main )
   , m_shader( NULL )
{
   m_depthBufferInput = new RPTextureInput( "DepthBuffer" );
   m_normalBufferInput = new RPTextureInput( "NormalBuffer" );
   m_lightTexture = new RPTextureOutput( "LightsTexture" );

   defineInput( m_depthBufferInput );
   defineInput( m_normalBufferInput );
   defineOutput( m_lightTexture );
}

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::~RPDeferredLightingNode()
{
   m_depthBufferInput = NULL;
   m_normalBufferInput = NULL;
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
   m_depthBufferInput = DynamicCast< RPTextureInput >( findInput( "DepthBuffer" ) );
   m_normalBufferInput = DynamicCast< RPTextureInput >( findInput( "NormalBuffer" ) );
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
   RuntimeDataBuffer& data = host.data();

   ShaderTexture* depthBufferTex = m_depthBufferInput->getValue( data );
   ShaderTexture* normalBufferTex = m_normalBufferInput->getValue( data );
   if ( !host.isSceneActive( m_renderedSceneId ) || !m_shader || !depthBufferTex || !normalBufferTex )
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
      comm->setTexture( "depthBuffer", *depthBufferTex );
      comm->setTexture( "normalBuffer", *normalBufferTex );
   }

   // render light volumes
   const Array< Light* >& visibleLights = host.getSceneLights( m_renderedSceneId );  
   uint lightsCount = visibleLights.size();
   for ( uint i = 0; i < lightsCount; ++i )
   {
      Light* light = visibleLights[i];
      light->render( renderer );
   }

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_shader );
}

///////////////////////////////////////////////////////////////////////////////
