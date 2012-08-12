#include "core-Renderer/RPPostProcessNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/FullscreenQuad.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/MRTUtil.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPPostProcessNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Pixel shader", PixelShader*, m_shader );
   PROPERTY_EDIT( "Render target ids", std::string, m_renderTargetId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPPostProcessNode::RPPostProcessNode()
   : m_shader( NULL )
   , m_shaderNode( NULL )
   , m_renderTargetId( "Color" )
{
   m_shaderNode = new PixelShaderNodeOperator< RenderingPipelineNode >( *this );

   MRTUtil::defineOutputs( m_renderTargetId, this );
}

///////////////////////////////////////////////////////////////////////////////

RPPostProcessNode::~RPPostProcessNode()
{
   delete m_shaderNode; m_shaderNode = NULL;
   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onPrePropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_shader" && m_shaderNode )
   {
      m_shaderNode->resetShader();
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_renderTargetId" )
   {
      MRTUtil::defineOutputs( m_renderTargetId, this );
   }
   else if ( property.getName() == "m_shader" && m_shader )
   {
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   if ( m_shader )
   {
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   data.registerVar( m_renderTargets );

   // create the render target
   {
      Array< RenderTarget* >* renderTargets = new Array< RenderTarget* >();
      data[ m_renderTargets ] = renderTargets;

      MRTUtil::refreshRenderTargets( host, this, *renderTargets );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !m_shader || !m_shaderNode )
   {
      return;
   }

   RuntimeDataBuffer& data = host.data();
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

   // bind the shader
   m_shaderNode->bindShader( renderer, data );

   // determine the quad size ( take any of the defined render targets, since
   // MRT requires them all to have the same size anyway ).
   unsigned int quadWidth, quadHeight;
   if ( renderTargets[0] )
   {
      quadWidth = renderTargets[0]->getWidth();
      quadHeight = renderTargets[0]->getHeight();
   }
   else
   {
      quadWidth = renderer.getViewportWidth();
      quadHeight = renderer.getViewportHeight();
   }
   
   new ( renderer() ) RCFullscreenQuad( quadWidth, quadHeight );

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_shader, renderer );

   // unbind render targets
   for ( uint i = 0; i < rtCount; ++i )
   {
      new ( renderer() ) RCDeactivateRenderTarget( i );
   }
}

///////////////////////////////////////////////////////////////////////////////
