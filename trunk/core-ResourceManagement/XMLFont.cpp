#include "core-ResourceManagement\XMLFont.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialOperation.h"
#include "tinyxml\tinyxml.h"
#include <string>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

XMLFont::XMLFont(const char* fontDefFile, 
                 const char* fontName, 
                 const Color& color, 
                 ResourceManager& resMgr)
      : m_resMgr(resMgr),
      m_glifs(256, NULL),
      m_glifSizes(256, 0)
{
   // parse the xml definition of the font
   TiXmlDocument document(fontDefFile);
   if (document.LoadFile() == false)
   {
      throw std::runtime_error(std::string("Cannot load file ") + fontDefFile);
   }
   TiXmlElement* root = document.FirstChildElement("Font"); 
   if (root == NULL) 
   {
      throw std::runtime_error(std::string(fontDefFile) + " doesn't contain valid font definition");
   }

   std::string fontFaceFileName = root->Attribute("fontFaceFileName");

   float size = 0;
   if (root->QueryFloatAttribute("size", &size) != TIXML_SUCCESS)
   {
      throw std::runtime_error("Invalid value in <size> attribute - float expected");
   }

   // create the material for the font
   char tmpMaterialName[256];
   sprintf_s(tmpMaterialName, "%s_mat", fontName);
   createMaterial(tmpMaterialName, fontFaceFileName.c_str(), color);

   // parse the glifs and create the entities for them
   char tmpGlifMeshName[256];
   unsigned char c;
   float u1, v1, u2, v2;
   float longestGlif = 0;
   unsigned int glifIdx = 0;
   for (TiXmlElement* glif = root->FirstChildElement("Glif"); 
        glif != NULL; glif = glif->NextSiblingElement("Glif"), ++glifIdx)
   {
      parseGlif(*glif, c, u1, v1, u2, v2);

      if (m_glifs[c] != NULL)
      {
         throw std::runtime_error("Character already defined");
      }

      float width = (u2 - u1) / size;
      float height = (v2 - v1) / size;

      if (width > longestGlif)
      {
         longestGlif = width;
      }

      float tu1 = u1 / size;
      float tv1 = v1 / size;
      float tu2 = u2 / size;
      float tv2 = v2 / size;

      sprintf_s(tmpGlifMeshName, "%s_glif_%d", fontName, c);

      AbstractGraphicalEntity& glifEntity = prepareEntity(tmpGlifMeshName, tmpMaterialName, 
                                                          width / 2.f, height / 2.f,
                                                          tu1, tv1,
                                                          tu2, tv2);
      m_glifs[c] = &glifEntity;
      m_glifSizes[c] = width;
   }

   // remember to always add a white space character
   sprintf_s(tmpGlifMeshName, "%s_glif_whiteSpace", fontName);
   AbstractGraphicalEntity& glifEntity = prepareEntity(tmpGlifMeshName, tmpMaterialName, 
                                                       (longestGlif / 4.f), 1,
                                                       0, 0, 0, 0);
   m_glifs[255] = &glifEntity;
   m_glifSizes[255] = (longestGlif / 2.f);
}

///////////////////////////////////////////////////////////////////////////////

void XMLFont::parseGlif(TiXmlElement& glif, 
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

void XMLFont::createMaterial(const char* materialName, 
                             const char* texName,
                             const Color& color)
{
   if (m_resMgr.doesMaterialExist(materialName)) {return;}

   Texture* texture = NULL;
   if (m_resMgr.isTextureRegistered(texName))
   {
      texture = &m_resMgr.getTexture(texName);
   }
   else
   {
      texture = &m_resMgr.loadTexture(texName);
   }

   LightReflectingProperties* lrp  = m_resMgr.createLightReflectingProperties();
   lrp->setAmbientColor(color);
   lrp->setDiffuseColor(color);
   lrp = &m_resMgr.addLightReflectingProperties(lrp);

   Material* realMat = &m_resMgr.createMaterial(materialName, *lrp);
   MaterialStage* defaultStage = m_resMgr.createMaterialStage(*texture,
      MOP_SELECT_ARG1, SC_LRP, SC_NONE,
      MOP_SELECT_ARG1, SC_TEXTURE, SC_NONE);
   realMat->addStage(defaultStage);
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity& XMLFont::prepareEntity(const char* meshName,
                                                const char* materialName,
                                                float width, float height,
                                                float tu1, float tv1,
                                                float tu2, float tv2)
{
   if (m_resMgr.isGraphicalEntityRegistered(meshName))
   {
      return m_resMgr.getGraphicalEntity(meshName);
   }
   else
   {
      MeshDefinition mesh;
      mesh.isSkin = false;
      mesh.name = meshName;
      D3DXMatrixIdentity(&(mesh.localMtx));
      mesh.vertices.push_back(LitVertex::unskinnedOneTex(-width, -height, 0, 0, 0, -1, tu1, tv2));
      mesh.vertices.push_back(LitVertex::unskinnedOneTex( width, -height, 0, 0, 0, -1, tu2, tv2));
      mesh.vertices.push_back(LitVertex::unskinnedOneTex(-width,  height, 0, 0, 0, -1, tu1, tv1));
      mesh.vertices.push_back(LitVertex::unskinnedOneTex( width,  height, 0, 0, 0, -1, tu2, tv1));
      mesh.faces.push_back(Face<USHORT>(0, 2, 1, 0));
      mesh.faces.push_back(Face<USHORT>(1, 2, 3, 0));

      MaterialDefinition material(materialName);
      mesh.materials.push_back(material);

      AbstractGraphicalEntity* ent = m_resMgr.createGraphicalEntityFromTemplate(mesh);
      m_resMgr.registerGraphicalEntity(meshName, ent);

      return *ent;
   }
}

///////////////////////////////////////////////////////////////////////////////

AbstractGraphicalEntity& XMLFont::getChar(unsigned char c)
{
   if (m_glifs[c] == NULL) 
   {
      return *(m_glifs[255]);
   }
   else
   {
      return *(m_glifs[c]);
   }
}

///////////////////////////////////////////////////////////////////////////////

float XMLFont::getCharWidth(unsigned char c) const
{
   if (m_glifs[c] == NULL) 
   {
      return m_glifSizes[255];
   }
   else
   {
      return m_glifSizes[c];
   }
}

///////////////////////////////////////////////////////////////////////////////
