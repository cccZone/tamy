/// @file   core-Renderer/ComputedRenderState.h
/// @brief  a render state used by the RPSBComputed scene tree builder
#pragma once

#include "core/MemoryPool.h"
#include "core-Renderer/RenderState.h"
#include "core/ReflectionObject.h"
#include "core-Renderer/PixelShaderNodeOperator.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class RenderingPipelineNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * A render state used by the RPSBComputed scene tree builder.
 */
class ComputedRenderState : public MemoryPoolObject, public TRenderState< ComputedRenderState >
{
   DECLARE_CLASS()

private:
   PixelShader&                                             m_shader;
   PixelShaderNodeOperator< RenderingPipelineNode >&        m_shaderNode;
   RuntimeDataBuffer&                                       m_data;

public:
   ComputedRenderState( PixelShader& shader, PixelShaderNodeOperator< RenderingPipelineNode >& shaderNode, RuntimeDataBuffer& data );

   // -------------------------------------------------------------------------
   // RenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer ) const;

   void onPostRender( Renderer& renderer ) const;

   inline bool onEquals( const ComputedRenderState& rhs ) const { return &rhs == this; }
   inline bool onLess( const ComputedRenderState& rhs ) const { return &rhs < this; }
};

///////////////////////////////////////////////////////////////////////////////
