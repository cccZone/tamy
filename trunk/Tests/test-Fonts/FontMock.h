#pragma once

#include "ext-Fonts\Font.h"
#include "GraphicalEntityMock.h"
#include "core-Renderer\Material.h"


///////////////////////////////////////////////////////////////////////////////

class FontMock : public Font
{
private:
   std::vector<unsigned char> m_chars;
   GraphicalEntityMock m_mockEntity;
   Material* m_material;

public:
   FontMock() 
      : Font("FontMock") 
   {
      m_material = new Material("someMaterial");
   }

   ~FontMock()
   {
      delete m_material;
      m_material = NULL;
   }

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

   const Material& getMaterial() const
   {
      return *m_material;
   }
};

///////////////////////////////////////////////////////////////////////////////
