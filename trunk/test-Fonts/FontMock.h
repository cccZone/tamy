#pragma once

#include "ext-Fonts\Font.h"
#include "GraphicalEntityMock.h"


///////////////////////////////////////////////////////////////////////////////

class FontMock : public Font
{
private:
   std::vector<unsigned char> m_chars;
   GraphicalEntityMock m_mockEntity;

public:
   FontMock() : Font("FontMock") {}

   void reset()
   {
      m_chars.clear();
   }

   AbstractGraphicalEntity& getChar(unsigned char c) 
   {
      m_chars.push_back(c);
      return m_mockEntity;
   }

   char getRequestedChar(unsigned int idx)
   {
      return m_chars.at(idx);
   }

   float getCharWidth(unsigned char c) const
   {
      return 10;
   }
};

///////////////////////////////////////////////////////////////////////////////
