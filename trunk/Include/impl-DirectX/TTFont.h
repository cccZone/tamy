#pragma once

#include "ext-Fonts\Font.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class AbstractGraphicalEntity;
struct Color;

///////////////////////////////////////////////////////////////////////////////

class TTFont : public Font
{
private:
   std::vector<AbstractGraphicalEntity*> m_glifs;
   std::vector<float> m_glifSizes;

public:
   TTFont(const char* fontFaceName, 
          const char* fontName, 
          const Color& color);

   AbstractGraphicalEntity& getChar(unsigned char c);

   float getCharWidth(unsigned char c) const;

   const Material& getMaterial() const;
};

///////////////////////////////////////////////////////////////////////////////
