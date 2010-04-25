#pragma once

#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include <d3dx9.h>
#include <windows.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

struct Color;
class FontImpl;

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
class Font : public TRendererObject<FontImpl>
{
   DECLARE_RTTI_CLASS

private:
   std::string m_name;
   D3DXFONT_DESC m_desc;

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
   Font(const D3DXFONT_DESC& desc);

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

   /**
    * The method draws a text on the currently set rendering target.
    *
    * @param text             text to be drawn
    * @param position         a window on the screen the text should be 
    *                         displayed in
    * @param justification    the way the text should be justified 
    *                         in the positioning window
    * @param color            color in which the text should be displayed
    */
   void drawText(const std::string& text,
                 RECT& position,
                 TextJustification justification,
                 const Color& color);
};

///////////////////////////////////////////////////////////////////////////////

class FontImpl : public RendererObjectImpl
{
public:
   virtual ~FontImpl() {}

   /**
    * The method draws a text on the currently set rendering target.
    *
    * @param text             text to be drawn
    * @param position         a window on the screen the text should be 
    *                         displayed in
    * @param justification    the way the text should be justified 
    *                         in the positioning window
    * @param color            color in which the text should be displayed
    */
   virtual void drawText(const std::string& text,
                         RECT& position,
                         TextJustification justification,
                         const Color& color) {}
};

///////////////////////////////////////////////////////////////////////////////
