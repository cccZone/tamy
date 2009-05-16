#pragma once

#include "core-ResourceManagement\MaterialXmlElement.h"


///////////////////////////////////////////////////////////////////////////////

class ResourceManager;
class LightReflectingProperties;

///////////////////////////////////////////////////////////////////////////////

class MaterialLeaf : public MaterialXmlElement
{
private:
   ResourceManager& m_resMgr;

public:
   MaterialLeaf(MaterialsParser& mainParser, ResourceManager& resMgr);

   void parse(TiXmlElement& parent);

private:
   void parseLRP(TiXmlElement& lrpElem, LightReflectingProperties& lrp);
};

///////////////////////////////////////////////////////////////////////////////
