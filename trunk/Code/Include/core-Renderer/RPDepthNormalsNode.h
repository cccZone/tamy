/// @file   core-Renderer/RPDepthNormalsNode.h
/// @brief  a rendering pipeline node responsible for rendering scene's depth & normals buffers
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/SceneRenderTreeBuilder.h"
#include "core-Renderer/RenderState.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class PixelShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node responsible for rendering scene's depth & normals buffers.
 */
class RPDepthNormalsNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPDepthNormalsNode )

private:
   class DepthNormalsRenderState : public TRenderState< DepthNormalsRenderState >, public SceneRenderTreeBuilder
   {
   private:
      PixelShader*         m_shader;

   public:
      /**
       * Constructor.
       *
       * @param shader
       */
      DepthNormalsRenderState();

      // -------------------------------------------------------------------------
      // RenderState implementation
      // -------------------------------------------------------------------------
      void onPreRender( Renderer& renderer );
      void onPostRender( Renderer& renderer );
      bool onEquals( const DepthNormalsRenderState& rhs ) const { return &rhs == this; }
      bool onLess( const DepthNormalsRenderState& rhs ) const { return &rhs < this; }

      // -------------------------------------------------------------------------
      // SceneRenderTreeBuilder implementation
      // -------------------------------------------------------------------------
      StateTreeNode* buildStateTree( const Array< SpatialRepresentation* >& visibleElems ) const;
   };

private:
   // static data
   RPMSceneId                                m_renderedSceneId;

   // runtime data
   TRuntimeVar< DepthNormalsRenderState* >   m_renderState;
   TRuntimeVar< RenderTarget* >              m_renderTarget;

public:
   RPDepthNormalsNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onInitialize( RenderingPipelineMechanism& host ) const;
   void onDeinitialize( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
