#include "impl-DirectX\D3DFont.h"
#include "impl-DirectX\D3DRenderer.h"
#include "core-Renderer\Color.h"


///////////////////////////////////////////////////////////////////////////////

D3DFont::D3DFont(const D3DXFONT_DESC& fontDesc,
                 IDirect3DDevice9& d3Device, 
                 D3DRenderer& renderer)
: Font(fontDesc.FaceName)
, m_d3Device(d3Device)
, m_renderer(renderer)
, m_font(NULL)
{
   HRESULT res = D3DXCreateFontIndirect(&m_d3Device, &fontDesc, &m_font);

   if ((FAILED(res)) || (m_font == NULL))
   {
      throw std::runtime_error(std::string("Font ") + fontDesc.FaceName + " could not be created");
   }

   dynamic_cast<Subject<D3DRenderer, D3DGraphResourceOp>& > (m_renderer).attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

D3DFont::~D3DFont()
{
   dynamic_cast<Subject<D3DRenderer, D3DGraphResourceOp>& > (m_renderer).detachObserver(*this);

   m_font->Release();
   m_font = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void D3DFont::drawText(const std::string& text,
                       RECT& position,
                       TextJustification justification,
                       const Color& color)
{
   static int textLen = -1;

   DWORD format = DT_WORDBREAK;
   switch(justification)
   {
   case TJ_LEFT:     format |= DT_LEFT;   break;
   case TJ_CENTERED: format |= DT_CENTER; break;
   case TJ_RIGHT:    format |= DT_RIGHT;  break;
   default: break;
   }

   m_font->DrawText(NULL,
                    text.c_str(), 
                    textLen, 
                    &position, 
                    format, 
                    D3DCOLOR_RGBA(((int)(color.r * 255)), 
                                  ((int)(color.g * 255)),
                                  ((int)(color.b * 255)),
                                  ((int)(color.a * 255))));
}

///////////////////////////////////////////////////////////////////////////////

void D3DFont::update(D3DRenderer& renderer)
{
   // do nothing
}

///////////////////////////////////////////////////////////////////////////////

void D3DFont::update(D3DRenderer& renderer, const D3DGraphResourceOp& operation)
{
   switch(operation)
   {
   case GRO_RELEASE_RES:
      {
         m_font->OnLostDevice();
         break;
      }
   
   case GRO_CREATE_RES:
      {
         m_font->OnResetDevice();
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
