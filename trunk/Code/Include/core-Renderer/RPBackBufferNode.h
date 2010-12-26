/// @file   core-Renderer/RPBackBufferNode.h
/// @brief  a rendering pipeline node responsible for managing the back buffer render target
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

class RPBackBufferNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPBackBufferNode )

private:
   // runtime data
   Renderer*            m_renderer;

public:
   RPBackBufferNode();
   ~RPBackBufferNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void initialize( RenderingPipelineMechanism& host );
   void deinitialize( RenderingPipelineMechanism& host );
   void update( RenderingPipelineMechanism& host );
};

///////////////////////////////////////////////////////////////////////////////
