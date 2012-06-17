/// @file   core-Renderer/RPSceneRenderNode.h
/// @brief  a rendering pipeline node responsible for rendering a scene
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"

#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class RPSceneBuilder;
class MemoryPool;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node responsible for rendering a scene.
 */
class RPSceneRenderNode : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   // static data
   RPMSceneId                          m_renderedSceneId;
   RPSceneBuilder*                     m_builder;
   std::string                         m_renderTargetId;
   bool                                m_clearDepthBuffer;

   // runtime data
   TRuntimeVar< MemoryPool* >          m_treeMemPool;
   TRuntimeVar< RenderTarget* >        m_renderTarget;

public:
   /**
    * Constructor.
    */
   RPSceneRenderNode();
   ~RPSceneRenderNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onDestroyLayout( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onPropertyChanged( ReflectionProperty& property );
};

///////////////////////////////////////////////////////////////////////////////
