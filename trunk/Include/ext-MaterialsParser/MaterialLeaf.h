#pragma once

#include "ext-MaterialsParser\MaterialXmlElement.h"
#include "core-Renderer\RenderingTechnique.h"
#include "core\ResourceStorage.h"


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
class LightReflectingProperties;

///////////////////////////////////////////////////////////////////////////////

class MaterialLeaf : public MaterialXmlElement
{
private:
   GraphicalEntitiesFactory& m_factory;
   ResourceStorage<RenderingTechnique>& m_storage;

public:
   MaterialLeaf(MaterialsParser& mainParser, 
                GraphicalEntitiesFactory& factory,
                ResourceStorage<RenderingTechnique>& storage);

   void parse(TiXmlElement& parent);

private:
   void parseLRP(TiXmlElement& lrpElem, LightReflectingProperties& lrp);
};

///////////////////////////////////////////////////////////////////////////////
