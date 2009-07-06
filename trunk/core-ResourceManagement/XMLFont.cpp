#include "core-ResourceManagement\XMLFont.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "tinyxml\tinyxml.h"
#include <string>
#include <stdexcept>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

XMLFontLoader::XMLFontLoader(const std::string& fileName, 
                             unsigned char requestedGlifIdx)
      : m_fileName(fileName),
      m_requestedGlifIdx(requestedGlifIdx),
      m_glifSize(0)
{
}

///////////////////////////////////////////////////////////////////////////////

void XMLFontLoader::parseMesh(MeshDefinition& mesh, 
                              AnimationDefinition& animation, 
                              std::vector<MaterialDefinition>& materials,
                              const std::string& name)
{
   // parse the xml definition of the font
   TiXmlDocument document(m_fileName.c_str());
   if (document.LoadFile() == false)
   {
      throw std::runtime_error(std::string("Cannot load file ") + m_fileName);
   }
   TiXmlElement* root = document.FirstChildElement("Font"); 
   if (root == NULL) 
   {
      throw std::runtime_error(m_fileName + " doesn't contain valid font definition");
   }

   std::string fontFaceFileName = root->Attribute("fontFaceFileName");

   float size = 0;
   if (root->QueryFloatAttribute("size", &size) != TIXML_SUCCESS)
   {
      throw std::runtime_error("Invalid value in <size> attribute - float expected");
   }

   // create the material for the font
   char tmpMaterialName[256];
   sprintf_s(tmpMaterialName, "%s_mat", m_fileName.c_str());
   materials.push_back(MaterialDefinition(tmpMaterialName));
   createMaterial(materials.back(), fontFaceFileName.c_str());

   // parse the glifs and create the entities for them
   char tmpGlifMeshName[256];
   unsigned char c;
   float u1, v1, u2, v2;
   unsigned int glifIdx = 0;
   TiXmlElement* glif = root->FirstChildElement("Glif"); 
   for (; (glif != NULL); 
        glif = glif->NextSiblingElement("Glif"), ++glifIdx)
   {
      parseGlif(*glif, c, u1, v1, u2, v2);
      if (c == m_requestedGlifIdx) {break;}
   }

   if (glif == NULL)
   {
      throw std::runtime_error("Glif not found");
   }

   float width = (u2 - u1) / size;
   float height = (v2 - v1) / size;
   float tu1 = u1 / size;
   float tv1 = v1 / size;
   float tu2 = u2 / size;
   float tv2 = v2 / size;

   sprintf_s(tmpGlifMeshName, "%s_glif_%d", m_fileName.c_str(), c);
   prepareEntity(mesh, tmpGlifMeshName, tmpMaterialName, 
                 width / 2.f, height / 2.f,
                 tu1, tv1,
                 tu2, tv2);
   m_glifSize = width;
}

///////////////////////////////////////////////////////////////////////////////

void XMLFontLoader::parseGlif(TiXmlElement& glif, 
                              unsigned char& c, 
                              float& u1, float& v1, 
                              float& u2, float& v2)
{
   std::string charVal = glif.Attribute("char");
   if (charVal.size() != 1)
   {
      throw std::runtime_error("Invalid value of <char> attribute - single character expected");
   }
   c = charVal[0];

   if (glif.QueryFloatAttribute("u1", &u1) != TIXML_SUCCESS)
   {
      throw std::runtime_error("Invalid value in <u1> attribute - float expected");
   }

   if (glif.QueryFloatAttribute("v1", &v1) != TIXML_SUCCESS)
   {
      throw std::runtime_error("Invalid value in <v1> attribute - float expected");
   }

   if (glif.QueryFloatAttribute("u2", &u2) != TIXML_SUCCESS)
   {
      throw std::runtime_error("Invalid value in <u2> attribute - float expected");
   }

   if (glif.QueryFloatAttribute("v2", &v2) != TIXML_SUCCESS)
   {
      throw std::runtime_error("Invalid value in <v2> attribute - float expected");
   }
}

///////////////////////////////////////////////////////////////////////////////

void XMLFontLoader::createMaterial(MaterialDefinition& material,
                                   const char* texName)
{
   material.texName = texName;
   material.ambient = Color(1, 1, 1, 1);
   material.diffuse = Color(1, 1, 1, 1);
   material.colorOp = MOP_SELECT_ARG1;
   material.colorArg1 = SC_LRP;
   material.colorArg2 = SC_NONE;
   material.alphaOp = MOP_SELECT_ARG1;
   material.alphaArg1 = SC_TEXTURE;
   material.alphaArg2 = SC_NONE;
}

///////////////////////////////////////////////////////////////////////////////

void XMLFontLoader::prepareEntity(MeshDefinition& mesh,
                                  const char* meshName,
                                  const char* materialName,
                                  float width, float height,
                                  float tu1, float tv1,
                                  float tu2, float tv2)
{
   mesh.isSkin = false;
   mesh.name = meshName;
   D3DXMatrixIdentity(&(mesh.localMtx));
   mesh.vertices.push_back(LitVertex::unskinnedOneTex(-width, -height, 0, 0, 0, -1, tu1, tv2));
   mesh.vertices.push_back(LitVertex::unskinnedOneTex( width, -height, 0, 0, 0, -1, tu2, tv2));
   mesh.vertices.push_back(LitVertex::unskinnedOneTex(-width,  height, 0, 0, 0, -1, tu1, tv1));
   mesh.vertices.push_back(LitVertex::unskinnedOneTex( width,  height, 0, 0, 0, -1, tu2, tv1));
   mesh.faces.push_back(Face<USHORT>(0, 2, 1, 0));
   mesh.faces.push_back(Face<USHORT>(1, 2, 3, 0));
   mesh.materials.push_back(materialName);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

std::string NameExtractor::operator()(const std::string& fullFileName)
{
   size_t dirMarkerPos = fullFileName.find_last_of("\\");
   size_t extMarkerPos = fullFileName.find_last_of(".");

   std::string name = fullFileName.substr(dirMarkerPos + 1, extMarkerPos - dirMarkerPos - 1);
   return name;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

XMLFont::XMLFont(const char* fontDefFile,  
                 GraphicalEntitiesFactory& entitiesFactory)
      : Font(NameExtractor()(fontDefFile)),
      m_entitiesFactory(entitiesFactory),
      m_glifs(256, NULL),
      m_glifSizes(256, -1)
{
   unsigned char glifsToParse[] = 
   {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 
    'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', 
    '8', '9', ';', ':', ',', '.', '\'', '/', '(', ')', '{', '}', '-', '=', ' ', 
    '[', ']', '\\', '|', '!', '@', '#', '$', '%', '^', '*'
   };
   unsigned int glifsCount = 86;

   GraphicalEntityLoader loader(entitiesFactory, m_materialsStorage);
   unsigned char glifIdx;
   for (unsigned int i = 0; i < glifsCount; ++i)
   {
      glifIdx = glifsToParse[i];

      std::stringstream name;
      name << "glif_" << glifIdx << "_" << fontDefFile;

      XMLFontLoader source(fontDefFile, glifIdx);
      AbstractGraphicalEntity* entity = loader.load(name.str(), source);

      m_glifs[glifIdx] = entity;
      m_glifSizes[glifIdx] = source.getGlifSize();
   }
}

///////////////////////////////////////////////////////////////////////////////

XMLFont::~XMLFont()
{
   unsigned int count = m_glifs.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_glifs[i];
   }
   m_glifs.clear();

   m_glifSizes.clear();
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity& XMLFont::getChar(unsigned char c)
{
   if (m_glifs[c] == NULL)
   {
      return *(m_glifs[32]);
   }
   else
   {
      return *(m_glifs[c]);
   }
}

///////////////////////////////////////////////////////////////////////////////

float XMLFont::getCharWidth(unsigned char c) const
{
   if (m_glifSizes[c] < 0)
   {
      return m_glifSizes[32];
   }
   else
   {
      return m_glifSizes[c];
   }
}

///////////////////////////////////////////////////////////////////////////////
