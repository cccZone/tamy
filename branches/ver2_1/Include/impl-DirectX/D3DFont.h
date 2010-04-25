#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include "core-Renderer\Font.h"
#include "core\Observer.h"


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer;
enum D3DGraphResourceOp;

///////////////////////////////////////////////////////////////////////////////

/**
 * A DirectX implementaiton of a font.
 */
class D3DFont : public Font, public Observer<D3DRenderer, D3DGraphResourceOp>
{
private:
   IDirect3DDevice9& m_d3Device;
   D3DRenderer& m_renderer;
   ID3DXFont* m_font;

public:
   D3DFont(const D3DXFONT_DESC& fontDesc,
           IDirect3DDevice9& d3Device, 
           D3DRenderer& renderer);
   ~D3DFont();

   // -------------------------------------------------------------------------
   // Font implementation
   // -------------------------------------------------------------------------

   void drawText(const std::string& text,
                 RECT& position,
                 TextJustification justification,
                 const Color& color);

   // -------------------------------------------------------------------------
   // Observer<D3DRenderer, D3DGraphResourceOp>
   // -------------------------------------------------------------------------
   void update(D3DRenderer& renderer);

   void update(D3DRenderer& renderer, const D3DGraphResourceOp& operation);
};

///////////////////////////////////////////////////////////////////////////////
