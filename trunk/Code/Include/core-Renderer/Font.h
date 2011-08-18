#pragma once

#include <d3dx9.h>
#include <windows.h>
#include <string>
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\RenderResource.h"
#include "core\Color.h"


///////////////////////////////////////////////////////////////////////////////

enum TextJustification
{
   TJ_LEFT,
   TJ_CENTERED,
   TJ_RIGHT
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface represents a font and its capabilities.
 */
class Font : public RenderResource
{
private:
   std::string          m_name;
   D3DXFONT_DESC        m_desc;

public:
   /**
    * Default constructor.
    */
   Font();

   /**
    * Constructor.
    *
    * @param desc    font description
    */
   Font( const D3DXFONT_DESC& desc );

   /**
    * Returns the name of the font. This method allows
    * to store fonts in ResourceStorages.
    *
    * @return                 font name
    */
   const std::string& getName() const;

   /**
    * Returns the font description.
    *
    * @return  font description
    */
   const D3DXFONT_DESC& getDescription() const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Text drawing command.
 */
class RCDrawText : public RenderCommand
{
private:
   Font&                m_font;
   std::string          m_text;
   RECT                 m_overlaySize;
   TextJustification    m_justification;
   Color                m_color;

public:
   RCDrawText( Font& font, const std::string& text, const RECT& overlaySize, TextJustification justification, const Color& color );

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////