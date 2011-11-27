/// @file   core-Renderer/RPSBComputed.h
/// @brief  use this builder if you want to pre-compute some scene aspect
#pragma once

#include "core-Renderer/RPSceneBuilder.h"
#include "core-Renderer/PixelShaderNodeOperator.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class RenderingPipelineNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Use this builder if you want to pre-compute some scene aspect using a shader,
 * like calculate scene normals or its depth for instance.
 */
class RPSBComputed : public RPSceneBuilder
{
   DECLARE_CLASS( RPSBComputed )

private:
   // static data
   PixelShader*                                                   m_shader;

   // runtime data
   PixelShaderNodeOperator< RenderingPipelineNode >*                   m_shaderNode;

public:
   /**
    * Constructor.
    */
   RPSBComputed();
   ~RPSBComputed();

   // -------------------------------------------------------------------------
   // SceneRenderTreeBuilder implementation
   // -------------------------------------------------------------------------
   StateTreeNode* buildRenderTree( MemoryPool& pool, const Array< Geometry* >& visibleElems, RuntimeDataBuffer& data ) const;

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
