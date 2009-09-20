#pragma once

#include <d3dx9.h>
#include <windows.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

struct Color;

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
class Font
{
private:
   std::string m_name;

public:
   virtual ~Font() {}

   /**
    * Returns the name of the font. This method allows
    * to store fonts in ResourceStorages.
    *
    * @return                 font name
    */
   const std::string& getName() const {return m_name;}

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
                         const Color& color) = 0;

protected:
   /**
    * Constructor.
    *
    * @param name             font name
    */
   Font(const std::string& name) : m_name(name) {}
};

///////////////////////////////////////////////////////////////////////////////
