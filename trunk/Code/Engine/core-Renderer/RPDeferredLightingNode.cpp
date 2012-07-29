#include "core-Renderer/RPDeferredLightingNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/Light.h"
#include "core-Renderer/MRTUtil.h"
#include "core/ResourcesManager.h"


// TODO: !!!!!!!!!!!!!!!!!  Expose alpha blending settings and z-buffer test settings through the pixel shader editor interface


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPDeferredLightingNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Render target ids", std::string, m_renderTargetId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::RPDeferredLightingNode()
   : m_renderTargetId( "LitScene" )
{
   m_depthNormalsInput = new RPTextureInput( "DepthNormals" );
   defineInput( m_depthNormalsInput );
   
   m_sceneColorInput = new RPTextureInput( "SceneColor" );
   defineInput( m_sceneColorInput );

   MRTUtil::defineOutputs( m_renderTargetId, this );
}

///////////////////////////////////////////////////////////////////////////////

RPDeferredLightingNode::~RPDeferredLightingNode()
{
   m_depthNormalsInput = NULL;
   m_sceneColorInput = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find input socket
   m_depthNormalsInput = DynamicCast< RPTextureInput >( findInput( "DepthNormals" ) );
   m_sceneColorInput = DynamicCast< RPTextureInput >( findInput( "SceneColor" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_renderTargetId" )
   {
      MRTUtil::defineOutputs( m_renderTargetId, this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_renderTargets );

   // create render targets
   {
      Array< RenderTarget* >* renderTargets = new Array< RenderTarget* >();
      data[ m_renderTargets ] = renderTargets;

      MRTUtil::refreshRenderTargets( host, this, *renderTargets );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPDeferredLightingNode::onDestroyLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   Array< RenderTarget* >* renderTargets = data[ m_renderTargets ];
   delete renderTargets;
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

   Array< RenderTarget* >& renderTargets = *(data[ m_renderTargets ]);
   if ( renderTargets.size() == 0 )
   {
      // nothing to render the scene to - bail
      return;
   }

   Renderer& renderer = host.getRenderer();

   // activate render targets we want to render to
   uint rtCount = renderTargets.size();
   for ( uint i = 0; i < rtCount; ++i )
   {
      new ( renderer() ) RCActivateRenderTarget( renderTargets[i], i );
   }

   // <lighting.todo>: !!!!!! once everything else works fine, use a regular scene builder here
   // to build a rendering tree and save on those pixel shader/vertex shader commands.
   // That will require that each light is built using GeometryEntity

   // render light volumes
   const Array< Light* >& visibleLights = host.getSceneLights();  
   uint lightsCount = visibleLights.size();
   for ( uint i = 0; i < lightsCount; ++i )
   {
      Light* light = visibleLights[i];
      light->render( renderer, depthNormalsTex, sceneColorTex );
   }

   // unbind render targets
   for ( uint i = 0; i < rtCount; ++i )
   {
      new ( renderer() ) RCDeactivateRenderTarget( i );
   }
}

///////////////////////////////////////////////////////////////////////////////
