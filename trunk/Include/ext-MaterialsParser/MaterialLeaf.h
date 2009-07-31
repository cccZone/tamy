#pragma once

#include "ext-MaterialsParser\MaterialXmlElement.h"
#include "core-Renderer\Material.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
class LightReflectingProperties;

///////////////////////////////////////////////////////////////////////////////

class MaterialLeaf : public MaterialXmlElement
{
private:
   GraphicalEntitiesFactory& m_factory;
   ResourceStorage<Material>& m_storage;

public:
   MaterialLeaf(MaterialsParser& mainParser, 
                GraphicalEntitiesFactory& factory,
                ResourceStorage<Material>& storage);

   void parse(TiXmlElement& parent);

private:
   void parseLRP(TiXmlElement& lrpElem, LightReflectingProperties& lrp);
};

///////////////////////////////////////////////////////////////////////////////
