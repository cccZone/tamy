#pragma once

#include "Texture.h"


///////////////////////////////////////////////////////////////////////////////

class TextureStub : public Texture
{
private:
   std::list<std::string>* m_messageSink;

public:
   TextureStub(const std::string& name) : Texture(name), m_messageSink(NULL) {}
   TextureStub(std::list<std::string>& messageSink) : Texture("textureStub"), m_messageSink(&messageSink) {}

   void setForRendering() 
   {
      if (m_messageSink != NULL)
      {
         m_messageSink->push_back("Set texture");
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
