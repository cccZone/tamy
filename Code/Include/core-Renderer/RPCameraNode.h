/// @file   core-Renderer/RPCameraNode.h
/// @brief  a node that provides information about the active camera
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RPFloatOutput;
class RPMatrixOutput;

///////////////////////////////////////////////////////////////////////////////

class RPCameraNode : public RenderingPipelineNode
{
   DECLARE_ALLOCATOR( RPCameraNode, AM_ALIGNED_16 );
   DECLARE_CLASS()

private:
   RPFloatOutput*    m_nearZ;
   RPFloatOutput*    m_farZ;
   RPFloatOutput*    m_viewportWidth;
   RPFloatOutput*    m_viewportHeight;
   RPMatrixOutput*   m_view;
   RPMatrixOutput*   m_invView;
   RPMatrixOutput*   m_invProj;

public:
   /**
    * Constructor.
    */
   RPCameraNode();
   ~RPCameraNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onUpdate( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
};

///////////////////////////////////////////////////////////////////////////////
