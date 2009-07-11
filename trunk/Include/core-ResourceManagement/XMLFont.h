#pragma once

#include "ext-Fonts\Font.h"
#include "core-ResourceManagement\GraphicalDataSource.h"
#include <vector>
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
class TiXmlElement;
class AbstractGraphicalEntity;
struct Color;
struct MaterialDefinition;
struct MeshDefinition;

///////////////////////////////////////////////////////////////////////////////

class XMLFontLoader : public GraphicalDataSource
{
private:
   std::string m_fileName;
   unsigned char m_requestedGlifIdx;
   float m_glifSize;

public:
   XMLFontLoader(const std::string& fileName, unsigned char requestedGlifIdx);

   float getGlifSize() const {return m_glifSize;}

   void parseMesh(MeshDefinition& mesh, 
                  AnimationDefinition& animation, 
                  std::vector<MaterialDefinition>& materials,
                  const std::string& name);

private:
   void parseGlif(TiXmlElement& glif, 
                  unsigned char& c, 
                  float& u1, float& v1, 
                  float& u2, float& v2);

   void prepareEntity(MeshDefinition& mesh,
                      const char* meshName,
                      float width, float height,
                      float tu1, float tv1,
                      float tu2, float tv2);
};

///////////////////////////////////////////////////////////////////////////////

class NameExtractor
{
public:
   std::string operator()(const std::string& fullFileName);
};

///////////////////////////////////////////////////////////////////////////////

class XMLFont : public Font
{
private:
   GraphicalEntitiesFactory& m_entitiesFactory;
   std::vector<AbstractGraphicalEntity*> m_glifs;
   std::vector<float> m_glifSizes;

   ResourceStorage<Material> m_materialsStorage;

public:
   XMLFont(const char* fontDefFile,
           GraphicalEntitiesFactory& entitiesFactory);
   ~XMLFont();

   const Material& getMaterial() const;

   AbstractGraphicalEntity& getChar(unsigned char c);

   float getCharWidth(unsigned char c) const;

private:
   Material* createMaterial(const char* fontDefFile);
};

///////////////////////////////////////////////////////////////////////////////
