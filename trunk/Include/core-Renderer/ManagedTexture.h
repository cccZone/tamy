#pragma once

#include "core-Renderer\Texture.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class manages the texture implementation it's created with
 * and doesn't allow for implementation changes
 */
class ManagedTexture : public Texture
{
private:
   TextureImpl* m_impl;

public:
   ManagedTexture(const std::string& name, TextureImpl* impl)
         : Texture(name),
         m_impl(impl)
   {
      if (m_impl == NULL)
      {
         throw std::invalid_argument("NULL pointer instead a TextureImpl instance");
      }
   }

   ~ManagedTexture()
   {
      delete m_impl; 
      m_impl = NULL;
   }

   TextureImpl& getImpl() {return *m_impl;}
};

///////////////////////////////////////////////////////////////////////////////
