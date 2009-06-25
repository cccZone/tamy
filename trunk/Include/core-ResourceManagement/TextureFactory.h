#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"
#include "core-ResourceManagement\FileResourceFactory.h"
#include "core-Renderer\Texture.h"
#include <string>
#include <map>


//////////////////////////////////////////////////////////////////////////////

class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<Texture> : public AbstractResourceFactory,
                         public FileResourceFactory<Texture>
{
private:
   std::string m_texturesDirName;

public:
   Factory(const std::string& texturesDirName) 
      : m_texturesDirName(texturesDirName) 
   {}

   virtual ~Factory() {}

protected:
   Texture* loadResource(const std::string& fileName)
   {
      if (fileName.length() > 0)
      {
         return load(m_texturesDirName, fileName);
      }
      else
      {
         return createEmpty();
      }
   }

   virtual Texture* load(const std::string& path, const std::string& fileName) = 0;
   virtual Texture* createEmpty() = 0;
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class TextureFactory : public Factory<Texture>
{
protected:
   ResourceManager& m_resMgr;

public:
   TextureFactory(ResourceManager& resMgr, const std::string& texturesDirName) 
         : Factory<Texture>(texturesDirName), 
         m_resMgr(resMgr) 
   {}

protected:
   Texture* load(const std::string& path, const std::string& fileName);
   Texture* createEmpty();
};

//////////////////////////////////////////////////////////////////////////////
