#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"
#include "core-ResourceManagement\FileResourceFactory.h"
#include "core-ResourceManagement\XMLFont.h"
#include "ext-Fonts\Font.h"
#include <string>


//////////////////////////////////////////////////////////////////////////////

struct Color;
class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<Font> : public AbstractResourceFactory,
                      public FileResourceFactory<Font>
{
private:
   ResourceManager& m_resMgr;
   std::string m_fontsDirPath;

public:
   Factory(ResourceManager& resMgr, const std::string& fontsDirPath) 
      : m_resMgr(resMgr), m_fontsDirPath(fontsDirPath) 
   {}

protected:
   Font* loadResource(const std::string& fileName)
   {
      XMLFont* font = new XMLFont((m_fontsDirPath + std::string("\\") + fileName).c_str(),
                               m_resMgr);
      return font;
   }
};

//////////////////////////////////////////////////////////////////////////////
