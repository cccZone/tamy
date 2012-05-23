/// @file   dx9-Renderer\DX9RenderTarget.h
/// @brief  directX9 implementation of a render target
#pragma once


#include "core-Renderer\RenderTarget.h"
#include "core\Color.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;
struct Point;

///////////////////////////////////////////////////////////////////////////////

class DX9RenderTarget
{
private:
   const  DX9Renderer&     m_renderer;
   const RenderTarget&     m_renderTarget;
   IDirect3DTexture9*      m_dxTexture;

public:
   DX9RenderTarget( const DX9Renderer& renderer, const RenderTarget& renderTarget );
   ~DX9RenderTarget();

   /**
    * Returns the color of the specified pixel.
    *
    * @param pos     pixel position
    */
   void getPixel( const Point& pos, Color& outColor ) const;

   /**
    * Returns the DirectX texture instance.
    */
   inline IDirect3DTexture9* getDxTexture() const { return m_dxTexture; }

   void onLostDevice();
   void onResetDevice();

private:
   void createTexture();
};

///////////////////////////////////////////////////////////////////////////////
