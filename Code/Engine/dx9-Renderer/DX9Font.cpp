#include "core-Renderer\Font.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core\Color.h"


///////////////////////////////////////////////////////////////////////////////

void RCDrawText::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );

   ID3DXFont* dxFont = dxRenderer.getFont( m_font );
   if ( !dxFont )
   {
      return;
   }

   // draw text
   static int textLen = -1;

   DWORD format = DT_WORDBREAK;
   switch( m_justification )
   {
   case TJ_LEFT:     format |= DT_LEFT;   break;
   case TJ_CENTERED: format |= DT_CENTER; break;
   case TJ_RIGHT:    format |= DT_RIGHT;  break;
   default: break;
   }

   dxFont->DrawText( NULL,
      m_text.c_str(), 
      textLen, 
      &m_overlaySize, 
      format, 
      D3DCOLOR_RGBA( ( ( int )( m_color.r * 255 ) ), 
      ( ( int )( m_color.g * 255 ) ),
      ( ( int )( m_color.b * 255 ) ),
      ( ( int )( m_color.a * 255 ) ) ) );
}

///////////////////////////////////////////////////////////////////////////////
