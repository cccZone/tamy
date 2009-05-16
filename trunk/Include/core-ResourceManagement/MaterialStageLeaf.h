#pragma once

#include "core-ResourceManagement\MaterialXmlElement.h"
#include "core-Renderer\MaterialOperation.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class ResourceManager;
class Texture;
class TiXmlElement;

///////////////////////////////////////////////////////////////////////////////

class MaterialStageLeaf : public MaterialXmlElement
{
private:
   ResourceManager& m_resMgr;

public:
   MaterialStageLeaf(MaterialsParser& mainParser, ResourceManager& resMgr);

   void parse(TiXmlElement& parent);

private:
   Texture& parseTexture(TiXmlElement& parent);

   void parseOperation(TiXmlElement& parent, 
                       const std::string& tag,
                       MatOpCode& opCode, 
                       SourceCode& arg1, 
                       SourceCode& arg2);

   MatOpCode getOperationFor(const std::string& val) const;
   SourceCode getSourceFor(const std::string& val) const;
};

///////////////////////////////////////////////////////////////////////////////
