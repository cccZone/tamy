#pragma once

#include "core-Renderer\Texture.h"


///////////////////////////////////////////////////////////////////////////////

class TextureStub : public Texture
{
private:
   std::list<std::string>* m_messageSink;

public:
   TextureStub(const std::string& name, bool isTransparent = false) 
      : Texture(name, isTransparent), m_messageSink(NULL) {}

   TextureStub(std::list<std::string>& messageSink, bool isTransparent = false) 
      : Texture("textureStub", isTransparent), m_messageSink(&messageSink) {}

   void setForRendering() 
   {
      if (m_messageSink != NULL)
      {
         m_messageSink->push_back("Set texture");
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
