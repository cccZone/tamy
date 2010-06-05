#pragma once

/// @file   dx9-Renderer\DX9RenderingPass.h
/// @brief  directX9 implementation of a rendering pass

#include "core-Renderer\RenderingPass.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////


class DX9RenderingPass : public RenderingPassImpl
{
private:
   RenderingPass&          m_renderingPass;
   IDirect3DDevice9*       m_d3Device;

public:
   DX9RenderingPass( RenderingPass& renderingPass );
   ~DX9RenderingPass();

   // -------------------------------------------------------------------------
   // RendererObjectImpl implementation
   // -------------------------------------------------------------------------
   void initialize(Renderer& renderer);

   // -------------------------------------------------------------------------
   // RenderingPassImpl implementation
   // -------------------------------------------------------------------------
   void passBegin();
   void passEnd();
};

///////////////////////////////////////////////////////////////////////////////
