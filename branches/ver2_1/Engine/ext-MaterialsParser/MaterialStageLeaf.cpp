#include "ext-MaterialsParser\MaterialStageLeaf.h"
#include "ext-MaterialsParser\MaterialsParser.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\Texture.h"


///////////////////////////////////////////////////////////////////////////////

MaterialStageLeaf::MaterialStageLeaf(MaterialsParser& mainParser,
                                     GraphicalEntitiesFactory& factory)
      : MaterialXmlElement(mainParser),
      m_factory(factory)
{
}

///////////////////////////////////////////////////////////////////////////////

void MaterialStageLeaf::parse(TiXmlElement& parent)
{
   if (m_mainParser.isMaterialParsed() == false)
   {
      throw std::runtime_error("Texture stage definition can only be embedded within \
                                a material definition");
   }

   std::string textureName = parseTexture(parent);

   MatOpCode  colorOpCode;
   SourceCode colorArg1;
   SourceCode colorArg2;
   MatOpCode  alphaOpCode;
   SourceCode alphaArg1;
   SourceCode alphaArg2;
   CoordsOpCode coordsOp;

   parseOperation(parent, "Color", colorOpCode, colorArg1, colorArg2);
   parseOperation(parent, "Alpha", alphaOpCode, alphaArg1, alphaArg2);
   parseOperation(parent, "Coords", coordsOp);

   MaterialStage* stage = m_factory.createMaterialStage(textureName, 
                                                        colorOpCode, colorArg1, colorArg2,
                                                        alphaOpCode, alphaArg1, alphaArg2,
                                                        coordsOp);


   Material& mat = m_mainParser.getMaterialParsed();
   mat.addStage(stage);
}

///////////////////////////////////////////////////////////////////////////////

std::string MaterialStageLeaf::parseTexture(TiXmlElement& parent)
{
   const char* tmpTextureName = parent.Attribute("texture");
   std::string textureName;
   if (tmpTextureName != NULL) 
   {
      textureName = tmpTextureName;
   }
   else
   {
      textureName = "";
   }

   return textureName;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialStageLeaf::parseOperation(TiXmlElement& parent, 
                                       const std::string& tag,
                                       MatOpCode& opCode, 
                                       SourceCode& arg1, 
                                       SourceCode& arg2)
{
   TiXmlElement* elem = parent.FirstChildElement(tag.c_str());
   if (elem != NULL)
   {
      const char* opName = elem->Attribute("op");
      if (opName == NULL)
      {
         throw std::runtime_error(std::string("Material stage misses an operation specification for ") + tag);
      }

      const char* arg1Val = elem->Attribute("arg1");
      if (arg1Val == NULL)
      {
         throw std::runtime_error(std::string("Material stage misses first argument specification for ") + tag);
      }

      const char* arg2Val = elem->Attribute("arg2");
      if (arg2Val == NULL)
      {
         throw std::runtime_error(std::string("Material stage misses second argument specification for ") + tag);
      }

      opCode = getOperationFor(opName);
      arg1 = getSourceFor(arg1Val);
      arg2 = getSourceFor(arg2Val);
   }
   else
   {
      opCode = MOP_DISABLE;
      arg1 = SC_NONE;
      arg2 = SC_NONE;
   }
}

///////////////////////////////////////////////////////////////////////////////

MatOpCode MaterialStageLeaf::getOperationFor(const std::string& val) const
{
   if (val == "disable")
   {
      return MOP_DISABLE;
   }
   else if (val == "arg1")
   {
      return MOP_SELECT_ARG1;
   }
   else if (val == "arg2")
   {
      return MOP_SELECT_ARG2;
   }
   else if (val == "mul")
   {
      return MOP_MULTIPLY;
   }
   else if (val == "mul2X")
   {
      return MOP_MULTIPLY_2X;
   }
   else if (val == "mul4X")
   {
      return MOP_MULTIPLY_4X;
   }
   else if (val == "add")
   {
      return MOP_ADD;
   }
   else if (val == "addSigned")
   {
      return MOP_ADD_SIGNED;
   }
   else if (val == "addSigned2X")
   {
      return MOP_ADD_SIGNED_2X;
   }
   else if (val == "addSmooth")
   {
      return MOP_ADD_SMOOTH;
   }
   else if (val == "sub")
   {
      return MOP_SUBTRACT;
   }
   else if (val == "mulAdd")
   {
      return MOP_MULTIPLY_ADD;
   }
   else
   {
      throw std::runtime_error(std::string("'") + val + "' is not a valid material operation");
   }

   return MOP_DISABLE;
}

///////////////////////////////////////////////////////////////////////////////

SourceCode MaterialStageLeaf::getSourceFor(const std::string& val) const
{
   if (val == "none")
   {
      return SC_NONE;
   }
   else if (val == "texture")
   {
      return SC_TEXTURE;
   }
   else if (val == "lrp")
   {
      return SC_LRP;
   }
   else if (val == "prev")
   {
      return SC_PREV;
   }
   else
   {
      throw std::runtime_error(std::string("'") + val + "' is not a valid material operation argument");
   }

   return SC_NONE;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialStageLeaf::parseOperation(TiXmlElement& parent,
                                       const std::string& tag,
                                       CoordsOpCode& coordsOp)
{
   const char* opName = parent.Attribute(tag.c_str());
   if (opName == NULL)
   {
      coordsOp = CC_WRAP;
   }
   else
   {
      std::string strOpName = opName;

      if (strOpName == "wrap")
      {
         coordsOp = CC_WRAP;
      }
      else if (strOpName == "mirror")
      {
         coordsOp = CC_MIRROR;
      }
      else if (strOpName == "clamp")
      {
         coordsOp = CC_CLAMP;
      }
      else if (strOpName == "border")
      {
         coordsOp = CC_BORDER;
      }
      else if (strOpName == "mirrorOnce")
      {
         coordsOp = CC_MIRRORONCE;
      }
      else
      {
         throw std::runtime_error(std::string("'") + strOpName + "' is not a valid coordinate operation argument");
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
