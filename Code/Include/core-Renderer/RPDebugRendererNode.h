/// @file   core-Renderer/RPDebugRendererNode.h
/// @brief  a rendering pipeline node that renders the debug info on the screen.
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"
#include "core/IDebugDraw.h"
#include "core/RuntimeData.h"
#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class RenderTarget;
class ModelDebugScene;
class Model;
class RenderingView;
class SceneRenderTreeBuilder;
class Geometry;
class MemoryPool;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline node that renders the debug info on the screen.
 */
class RPDebugRendererNode : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   // static data
   std::string                                     m_renderTargetId;

   SceneRenderTreeBuilder*                         m_builder;

   // runtime data
   typedef Array< Geometry* >                      GeometryArray;

   TRuntimeVar< MemoryPool* >                      m_treeMemPool;
   TRuntimeVar< RenderTarget* >                    m_renderTarget;
   TRuntimeVar< Model* >                           m_debugScene;
   TRuntimeVar< ModelDebugScene* >                 m_debugSceneView;
   TRuntimeVar< RenderingView* >                   m_renderingView;
   TRuntimeVar<  GeometryArray* >                  m_visibleElems;

public:
   /**
    * Constructor.
    */
   RPDebugRendererNode();
   ~RPDebugRendererNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onDestroyLayout( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
