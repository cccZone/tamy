#pragma once

#include <string>
#include "core-Renderer\RenderingTechnique.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
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
   MaterialsParser(GraphicalEntitiesFactory& factory, 
                   ResourceStorage<RenderingTechnique>& storage);
   ~MaterialsParser();

   void load(const std::string& fileName);

   bool isMaterialParsed() const;
   Material& getMaterialParsed();
   void setMaterialParsed(Material& mat);
};

///////////////////////////////////////////////////////////////////////////////
