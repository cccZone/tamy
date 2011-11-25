#include "core-Renderer/RPPostProcessNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/FullscreenQuad.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/PixelShaderConstant.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPPostProcessNode, RenderingPipelineNode );
   PROPERTY_EDIT( "Pixel shader", PixelShader*, m_shader );
   PROPERTY_EDIT( "Render target id", std::string, m_renderTargetId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPPostProcessNode::RPPostProcessNode()
   : m_shader( NULL )
   , m_shaderNode( NULL )
{
   defineOutput( new RPTextureOutput( "Output" ) );

   m_shaderNode = new ShaderNodeOperator< RenderingPipelineNode >( *this );
}

///////////////////////////////////////////////////////////////////////////////

RPPostProcessNode::~RPPostProcessNode()
{
   delete m_shaderNode; m_shaderNode = NULL;
   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onPrePropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_shader" && m_shaderNode )
   {
      m_shaderNode->resetShader();
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onPropertyChanged( Property& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_shader" && m_shader )
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
   host.data().registerVar( m_renderTarget );

   host.data()[ m_renderTarget ] = &host.getRenderTarget( m_renderTargetId );
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !m_shader || !m_shaderNode )
   {
      return;
   }

   RuntimeDataBuffer& data = host.data();
   RenderTarget* trg = data[ m_renderTarget ];
   Renderer& renderer = host.getRenderer();

   // bind the shader
   m_shaderNode->bindShader( renderer, data );

   // render the quad
   renderQuad( renderer, trg );

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_shader );
}

///////////////////////////////////////////////////////////////////////////////

void RPPostProcessNode::renderQuad( Renderer& renderer, RenderTarget* rt ) const
{
   // get the render target size
   unsigned int trgWidth, trgHeight;
   if ( rt )
   {
      trgWidth = rt->getWidth();
      trgHeight = rt->getHeight();
   }
   else
   {
      trgWidth = renderer.getViewportWidth();
      trgHeight = renderer.getViewportHeight();
   }

   new ( renderer() ) RCActivateRenderTarget( rt );
   new ( renderer() ) RCFullscreenQuad( trgWidth, trgHeight );
}

///////////////////////////////////////////////////////////////////////////////
