/// @file   core-Renderer/RPSBComputed.h
/// @brief  use this builder if you want to pre-compute some scene aspect
#pragma once

#include "core-Renderer/RPSceneBuilder.h"
#include "core-Renderer/RenderState.h"
#include "core-Renderer/ShaderNodeOperator.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this builder if you want to pre-compute some scene aspect using a shader,
 * like calculate scene normals or its depth for instance.
 */
class RPSBComputed : public RPSceneBuilder, public TRenderState< RPSBComputed >
{
   DECLARE_CLASS( RPSBComputed )

private:
   // static data
   PixelShader*                                          m_shader;

   // runtime data
   ShaderNodeOperator< RenderingPipelineNode >*          m_shaderNode;

public:
   /**
    * Constructor.
    */
   RPSBComputed();
   ~RPSBComputed();

   // -------------------------------------------------------------------------
   // SceneRenderTreeBuilder implementation
   // -------------------------------------------------------------------------
   StateTreeNode* buildRenderTree( MemoryPool& pool, const Array< SpatialRepresentation* >& visibleElems ) const;

   // -------------------------------------------------------------------------
   // RenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer, RuntimeDataBuffer& data ) const;
   void onPostRender( Renderer& renderer, RuntimeDataBuffer& data ) const;
   bool onEquals( const RPSBComputed& rhs ) const { return &rhs == this; }
   bool onLess( const RPSBComputed& rhs ) const { return &rhs < this; }

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPrePropertyChanged( Property& property );
   void onPropertyChanged( Property& property );

protected:
   // -------------------------------------------------------------------------
   // RPSceneBuilder implementation
   // -------------------------------------------------------------------------
   void onDefineSockets( RPSceneRenderNode& hostNode );
};

///////////////////////////////////////////////////////////////////////////////
