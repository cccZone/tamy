#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class ResourceManager;
class MultipleElement;
class Material;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class loads the material definitions from the XML file
 * and adds them to a ResourceManager instance
 */
class MaterialsParser
{
private:
   MultipleElement* m_parser;

   Material* m_parsedMaterial;

public:
   MaterialsParser(ResourceManager& resMgr);
   ~MaterialsParser();

   void load(const std::string& fileName);

   bool isMaterialParsed() const;
   Material& getMaterialParsed();
   void setMaterialParsed(Material& mat);
};

///////////////////////////////////////////////////////////////////////////////
