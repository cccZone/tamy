#pragma once

/// @file   dx9-Renderer\DX9RenderingPipelineMechanism.h
/// @brief  directX9 implementation of a rendering pipeline mechanism

#include "core-Renderer\RenderingPipelineMechanism.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////


class DX9RenderingPipelineMechanism : public RenderingPipelineMechanismImpl
{
private:
   RenderingPipelineMechanism&         m_renderingPipelineMechanism;
   IDirect3DDevice9*                   m_d3Device;

public:
   DX9RenderingPipelineMechanism( RenderingPipelineMechanism& renderingPipelineMechanism );
   ~DX9RenderingPipelineMechanism();

   // -------------------------------------------------------------------------
   // RendererObjectImpl implementation
   // -------------------------------------------------------------------------
   void initialize(Renderer& renderer);

   // -------------------------------------------------------------------------
   // RenderingPipelineMechanismImpl implementation
   // -------------------------------------------------------------------------
   void passBegin();
   void passEnd();
};

///////////////////////////////////////////////////////////////////////////////
