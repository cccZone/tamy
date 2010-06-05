#pragma once

/// @file   dx9-Renderer\DX9RenderTarget.h
/// @brief  directX9 implementation of a render target

#include "core-Renderer\RenderTarget.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;
enum DX9GraphResourceOp;

///////////////////////////////////////////////////////////////////////////////


class DX9RenderTarget : public RenderTargetImpl,
                        public Observer< DX9Renderer, DX9GraphResourceOp >
{
private:
   RenderTarget&        m_renderTarget;
   DX9Renderer*         m_renderer;
   IDirect3DTexture9*   m_dxTexture;

public:
   DX9RenderTarget( RenderTarget& renderTarget );
   ~DX9RenderTarget();

   // -------------------------------------------------------------------------
   // RendererObjectImpl implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );

   // -------------------------------------------------------------------------
   // RenderTargetImpl implementation
   // -------------------------------------------------------------------------
   void* getPlatformSpecific() const;
   Color getPixel( const D3DXVECTOR2& pos ) const;

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( DX9Renderer& renderer );
   void update( DX9Renderer& renderer, const DX9GraphResourceOp& operation );

private:
   void createTexture( DX9Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
