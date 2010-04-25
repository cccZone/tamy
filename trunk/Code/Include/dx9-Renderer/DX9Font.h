#pragma once

/// @file   dx9-Renderer\Font.h
/// @brief  DirectX9 font implementation
#include <d3dx9.h>
#include <d3d9.h>
#include "core-Renderer\Font.h"
#include "core\Observer.h"


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;
enum DX9GraphResourceOp;

///////////////////////////////////////////////////////////////////////////////

/**
 * A DirectX9 font implementation.
 */
class DX9Font : public FontImpl, 
                public Observer<DX9Renderer, DX9GraphResourceOp>
{
private:
   Font& m_font;
   DX9Renderer* m_renderer;
   ID3DXFont* m_dxFont;

public:
   DX9Font(Font& font);
   ~DX9Font();

   // -------------------------------------------------------------------------
   // RendererObjectImpl implementation
   // -------------------------------------------------------------------------
   void initialize(Renderer& renderer);

   // -------------------------------------------------------------------------
   // FontImpl implementation
   // -------------------------------------------------------------------------
   void drawText(const std::string& text,
                 RECT& position,
                 TextJustification justification,
                 const Color& color);

   // -------------------------------------------------------------------------
   // Observer<DX9Renderer, DX9GraphResourceOp>
   // -------------------------------------------------------------------------
   void update(DX9Renderer& renderer);

   void update(DX9Renderer& renderer, const DX9GraphResourceOp& operation);
};

///////////////////////////////////////////////////////////////////////////////
