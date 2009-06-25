#include "core-ResourceManagement\MaterialsParser.h"
#include "core-ResourceManagement\CompositeElement.h"
#include "core-ResourceManagement\MultipleElement.h"
#include "core-ResourceManagement\MaterialLeaf.h"
#include "core-ResourceManagement\MaterialStageLeaf.h"
#include "tinyxml\tinyxml.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

MaterialsParser::MaterialsParser(ResourceManager& resMgr)
      : m_parser(NULL),
      m_parsedMaterial(NULL)
{
   CompositeElement* materialElem = new CompositeElement(*this);
      materialElem->addElement(new MaterialLeaf(*this, resMgr));

      MultipleElement* stageElem = new MultipleElement(*this, "Stage", 
                                       new MaterialStageLeaf(*this, resMgr));
      materialElem->addElement(stageElem);
   m_parser = new MultipleElement(*this, "Material", materialElem);
}

///////////////////////////////////////////////////////////////////////////////

MaterialsParser::~MaterialsParser()
{
   m_parsedMaterial = NULL;

   delete m_parser;
   m_parser = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialsParser::load(const std::string& fileName)
{
   TiXmlDocument document(fileName.c_str());
   if (document.LoadFile() == false)
   {
      throw std::runtime_error(std::string("Cannot load file ") + fileName);
   }
   TiXmlElement* root = document.FirstChildElement("Root"); 
   if (root == NULL) 
   {
      throw std::runtime_error(fileName + " doesn't contain valid material definition");
   }

   m_parsedMaterial = NULL;

   m_parser->parse(*root);

   m_parsedMaterial = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool MaterialsParser::isMaterialParsed() const
{
   return m_parsedMaterial != NULL;
}

///////////////////////////////////////////////////////////////////////////////

Material& MaterialsParser::getMaterialParsed()
{
   return *m_parsedMaterial;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialsParser::setMaterialParsed(Material& mat)
{
   m_parsedMaterial = &mat;
}

///////////////////////////////////////////////////////////////////////////////
