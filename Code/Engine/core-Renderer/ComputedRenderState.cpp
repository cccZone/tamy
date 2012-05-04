#include "core-Renderer/ComputedRenderState.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( ComputedRenderState );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

ComputedRenderState::ComputedRenderState( PixelShader& shader, PixelShaderNodeOperator< RenderingPipelineNode >& shaderNode, RuntimeDataBuffer& data )
   : m_shader( shader )
   , m_shaderNode( shaderNode )
   , m_data( data )
{
}

///////////////////////////////////////////////////////////////////////////////

void ComputedRenderState::onPreRender( Renderer& renderer ) const
{
   m_shaderNode.bindShader( renderer, m_data );
}

///////////////////////////////////////////////////////////////////////////////

void ComputedRenderState::onPostRender( Renderer& renderer ) const
{
   new ( renderer() ) RCUnbindPixelShader( m_shader );
}

///////////////////////////////////////////////////////////////////////////////
