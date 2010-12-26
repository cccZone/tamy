/// @file   core-Renderer/RPRenderTargetNode.h
/// @brief  a rendering pipeline node responsible for managing a render target
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

class RPRenderTargetNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPRenderTargetNode )

private:
   std::string          m_renderTargetId;

   // runtime data
   RenderTarget*        m_renderTarget;
   Renderer*            m_renderer;

public:
   RPRenderTargetNode();
   ~RPRenderTargetNode();

   /**
    * Sets the target id.
    *
    * @param id      render target id
    */
   inline void setRenderTargetID( const std::string& id ) { m_renderTargetId = id; }

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void initialize( RenderingPipelineMechanism& host );
   void deinitialize( RenderingPipelineMechanism& host );
   void update( RenderingPipelineMechanism& host );
};

///////////////////////////////////////////////////////////////////////////////
