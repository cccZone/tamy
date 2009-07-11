#pragma once

#include "ext-MaterialsParser\MaterialXmlElement.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
class Texture;
class TiXmlElement;

///////////////////////////////////////////////////////////////////////////////

class MaterialStageLeaf : public MaterialXmlElement
{
private:
   GraphicalEntitiesFactory& m_factory;

public:
   MaterialStageLeaf(MaterialsParser& mainParser, GraphicalEntitiesFactory& factory);

   void parse(TiXmlElement& parent);

private:
   std::string parseTexture(TiXmlElement& parent);

   void parseOperation(TiXmlElement& parent, 
                       const std::string& tag,
                       MatOpCode& opCode, 
                       SourceCode& arg1, 
                       SourceCode& arg2);

   void parseOperation(TiXmlElement& parent,
                       const std::string& tag,
                       CoordsOpCode& coordsOp);

   MatOpCode getOperationFor(const std::string& val) const;
   SourceCode getSourceFor(const std::string& val) const;
};

///////////////////////////////////////////////////////////////////////////////
