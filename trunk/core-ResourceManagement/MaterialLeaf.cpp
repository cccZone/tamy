#include "core-ResourceManagement\MaterialLeaf.h"
#include "core-ResourceManagement\MaterialsParser.h"
#include "core-ResourceManagement\ResourceManager.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Texture.h"
#include "core-ResourceManagement\LightReflectingPropertiesFactory.h"
#include "core-ResourceManagement\MaterialFactory.h"


///////////////////////////////////////////////////////////////////////////////

MaterialLeaf::MaterialLeaf(MaterialsParser& mainParser, ResourceManager& resMgr)
      : MaterialXmlElement(mainParser),
      m_resMgr(resMgr)
{
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialLeaf::parse(TiXmlElement& parent)
{
   const char* materialName = parent.Attribute("name");
   if (materialName == NULL) 
   {
      throw std::runtime_error("Material needs to have a name assigned");
   }

   if (m_resMgr.resource<Material>().is(materialName))
   {
      return false;
   }
   else
   {
      // parse LRP
      LightReflectingProperties* lrp = m_resMgr.resource<LightReflectingProperties>()();
      TiXmlElement* lrpElem = parent.FirstChildElement("LightReflectingProperties");
      if (lrpElem == NULL)
      {
         lrp->setAmbientColor(Color());
         lrp->setDiffuseColor(Color(1, 1, 1, 1));
         lrp->setSpecularColor(Color(1, 1, 1, 1));
         lrp->setEmissiveColor(Color());
         lrp->setPower(1);
      }
      else
      {
         parseLRP(*lrpElem, *lrp);
      }

      // create the material
      Material& mat = m_resMgr.resource<Material>()(materialName, lrp);
      m_mainParser.setMaterialParsed(mat);
      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialLeaf::parseLRP(TiXmlElement& lrpElem, LightReflectingProperties& lrp)
{
   TiXmlHandle lrpHandle(&lrpElem);

   TiXmlElement* ambientElem = lrpHandle.FirstChildElement("Ambient").ToElement();
   Color ambient;
   if (ambientElem != NULL)
   {
      if ((ambientElem->QueryFloatAttribute("r", &ambient.r) != TIXML_SUCCESS) ||
          (ambientElem->QueryFloatAttribute("g", &ambient.g) != TIXML_SUCCESS) ||
          (ambientElem->QueryFloatAttribute("b", &ambient.b) != TIXML_SUCCESS) ||
          (ambientElem->QueryFloatAttribute("a", &ambient.a) != TIXML_SUCCESS))
      {
         throw std::runtime_error("Invalid ambient color value");
      }
      lrp.setAmbientColor(ambient);
   }

   TiXmlElement* diffuseElem = lrpHandle.FirstChildElement("Diffuse").ToElement();
   if (diffuseElem != NULL)
   {
      Color diffuse;
      if ((diffuseElem->QueryFloatAttribute("r", &diffuse.r) != TIXML_SUCCESS) ||
          (diffuseElem->QueryFloatAttribute("g", &diffuse.g) != TIXML_SUCCESS) ||
          (diffuseElem->QueryFloatAttribute("b", &diffuse.b) != TIXML_SUCCESS) ||
          (diffuseElem->QueryFloatAttribute("a", &diffuse.a) != TIXML_SUCCESS))
      {
         throw std::runtime_error("Invalid diffuse color value");
      }
      lrp.setDiffuseColor(diffuse);
   }

   TiXmlElement* specularElem = lrpHandle.FirstChildElement("Specular").ToElement();
   if (specularElem != NULL)
   {
      Color specular;
      float power;
      if ((specularElem->QueryFloatAttribute("r", &specular.r) != TIXML_SUCCESS) ||
          (specularElem->QueryFloatAttribute("g", &specular.g) != TIXML_SUCCESS) ||
          (specularElem->QueryFloatAttribute("b", &specular.b) != TIXML_SUCCESS) ||
          (specularElem->QueryFloatAttribute("a", &specular.a) != TIXML_SUCCESS) ||
          (specularElem->QueryFloatAttribute("power", &power) != TIXML_SUCCESS))
      {
         throw std::runtime_error("Invalid specular color value");
      }
      lrp.setSpecularColor(specular);
      lrp.setPower(power);
   }

   TiXmlElement* emissiveElem = lrpHandle.FirstChildElement("Emissive").ToElement();
   if (emissiveElem != NULL)
   {
      Color emissive;
      if ((emissiveElem->QueryFloatAttribute("r", &emissive.r) != TIXML_SUCCESS) ||
          (emissiveElem->QueryFloatAttribute("g", &emissive.g) != TIXML_SUCCESS) ||
          (emissiveElem->QueryFloatAttribute("b", &emissive.b) != TIXML_SUCCESS) ||
          (emissiveElem->QueryFloatAttribute("a", &emissive.a) != TIXML_SUCCESS))
      {
         throw std::runtime_error("Invalid emissive color value");
      }
      lrp.setEmissiveColor(emissive);
   }
}

///////////////////////////////////////////////////////////////////////////////
