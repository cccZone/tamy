#include "dx9-Renderer\DX9Font.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core\Color.h"


///////////////////////////////////////////////////////////////////////////////

DX9Font::DX9Font(Font& font)
: m_font(font)
, m_renderer(NULL)
, m_dxFont(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

DX9Font::~DX9Font()
{
   if (m_renderer != NULL)
   {
      dynamic_cast<Subject<DX9Renderer, DX9GraphResourceOp>*> (m_renderer)->detachObserver(*this);
      m_renderer = NULL;
   }

   if (m_dxFont != NULL)
   {
      m_dxFont->Release();
      m_dxFont = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9Font::initialize(Renderer& renderer)
{
   m_renderer = dynamic_cast<DX9Renderer*> (&renderer);
   if (m_renderer == NULL)
   {
      throw std::runtime_error("This implementation can work only with DX9Renderer");
   }

   // create the font
   HRESULT res = D3DXCreateFontIndirect(&(m_renderer->getD3Device()), 
                                        &(m_font.getDescription()), 
                                        &m_dxFont);

   if ((FAILED(res)) || (m_dxFont == NULL))
   {
      throw std::runtime_error(std::string("Font ") + m_font.getName() + " could not be created");
   }

   // attach the instance as the renderer observer
   dynamic_cast<Subject<DX9Renderer, DX9GraphResourceOp>*>(m_renderer)->attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

void DX9Font::drawText(const std::string& text,
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

   m_dxFont->DrawText(NULL,
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

void DX9Font::update(DX9Renderer& renderer)
{
   // do nothing
}

///////////////////////////////////////////////////////////////////////////////

void DX9Font::update(DX9Renderer& renderer, const DX9GraphResourceOp& operation)
{
   switch(operation)
   {
   case GRO_RELEASE_RES:
      {
         m_dxFont->OnLostDevice();
         break;
      }
   
   case GRO_CREATE_RES:
      {
         m_dxFont->OnResetDevice();
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
