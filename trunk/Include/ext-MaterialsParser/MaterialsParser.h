#pragma once

#include <string>
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
class MultipleElement;
class Material;
class File;

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
                   ResourceStorage<Material>& storage);
   ~MaterialsParser();

   void load(File* file);

   bool isMaterialParsed() const;
   Material& getMaterialParsed();
   void setMaterialParsed(Material& mat);
};

///////////////////////////////////////////////////////////////////////////////
