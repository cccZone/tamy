#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include <deque>


///////////////////////////////////////////////////////////////////////////////

GraphicalEntitiesFactory::GraphicalEntitiesFactory(const std::string& texturesPath)
      : m_texturesPath(texturesPath)
{
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntitiesFactory::~GraphicalEntitiesFactory()
{
}

///////////////////////////////////////////////////////////////////////////////

SkyBox* GraphicalEntitiesFactory::createSkyBox(Material& front, Material& back,
                                               Material& left, Material& right,
                                               Material& top, Material& bottom)
{
   SkyBox* skyBox = createSkyBox();
   skyBox->setMaterial(SBS_FRONT, front);
   skyBox->setMaterial(SBS_BACK, back);
   skyBox->setMaterial(SBS_LEFT, left);
   skyBox->setMaterial(SBS_RIGHT, right);
   skyBox->setMaterial(SBS_TOP, top);
   skyBox->setMaterial(SBS_BOTTOM, bottom);

   return skyBox;
}

///////////////////////////////////////////////////////////////////////////////

MaterialStage* GraphicalEntitiesFactory::createMaterialStage(
                  const std::string& textureName,
                  MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                  MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2,
                  CoordsOpCode coordsOperation)
{
   Texture* texture = NULL;
   if (m_textures.is(textureName))
   {
      texture = &(m_textures.get(textureName));
   }
   else
   {
      if (textureName.length() == 0)
      {
         texture = createEmptyTexture();
      }
      else
      {
         texture = loadTexture(m_texturesPath, textureName);
      }
   }

   return createMaterialStage(*texture,
                              colorOp, colorArg1, colorArg2,
                              alphaOp, alphaArg1, alphaArg2,
                              coordsOperation);
}

///////////////////////////////////////////////////////////////////////////////

Material* GraphicalEntitiesFactory::createMaterial(
                                          const std::string& materialName,
                                          LightReflectingProperties* lrp)
{
   Material* mat = createMaterialImpl(materialName, lrp);
   return mat;
}

///////////////////////////////////////////////////////////////////////////////
