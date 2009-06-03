#pragma once

#include "ext-Fonts\Font.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class ResourceManager;
class TiXmlElement;
class AbstractGraphicalEntity;
struct Color;

///////////////////////////////////////////////////////////////////////////////

class XMLFont : public Font
{
private:
   ResourceManager& m_resMgr;
   std::vector<AbstractGraphicalEntity*> m_glifs;
   std::vector<float> m_glifSizes;

public:
   XMLFont(const char* fontDefFile, 
           const char* fontName, 
           const Color& color, 
           ResourceManager& resMgr);

   AbstractGraphicalEntity& getChar(unsigned char c);

   float getCharWidth(unsigned char c) const;

private:
   void parseGlif(TiXmlElement& glif, 
                  unsigned char& c, 
                  float& u1, float& v1, 
                  float& u2, float& v2);

   void createMaterial(const char* materialName, 
                       const char* texName,
                       const Color& color);

   AbstractGraphicalEntity& prepareEntity(const char* meshName,
                                          const char* materialName,
                                          float width, float height,
                                          float tu1, float tv1,
                                          float tu2, float tv2);
};

///////////////////////////////////////////////////////////////////////////////
