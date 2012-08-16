/// @file   dx9-Renderer/DX9DepthBuffer.h
/// @brief  DX9 depth buffer implementation
#pragma once

#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DepthBuffer;
class DX9Renderer;

///////////////////////////////////////////////////////////////////////////////

class DX9DepthBuffer
{
private:
   const DX9Renderer&      m_renderer;
   const DepthBuffer&      m_depthBuffer;

   IDirect3DSurface9*      m_depthSurface;
   bool                    m_isActive;

public:
   /**
    * Constructor.
    *
    * @param renderer
    * @param depthBuffer
    */
   DX9DepthBuffer( const DX9Renderer& renderer, const DepthBuffer& depthBuffer );
   ~DX9DepthBuffer();

   /**
    * Activates this depth buffer.
    */
   void activate();

   /**
    * Deactivates this depth buffer.
    */
   void deactivate();

   // -------------------------------------------------------------------------
   // Device events handlers
   // -------------------------------------------------------------------------
   void onLostDevice();
   void onResetDevice();

private:
   void createDepthBuffer();
   void bind();
};


///////////////////////////////////////////////////////////////////////////////