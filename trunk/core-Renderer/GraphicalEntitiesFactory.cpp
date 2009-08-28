#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\SkyBoxStorage.h"
#include "core-Renderer\SkyBoxSide.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Light.h"
#include <deque>


///////////////////////////////////////////////////////////////////////////////

GraphicalEntitiesFactory::GraphicalEntitiesFactory(const std::string& texturesPath,
                                                   Renderer& renderer)
      : m_texturesPath(texturesPath),
      m_renderer(renderer)
{
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntitiesFactory::~GraphicalEntitiesFactory()
{
}

///////////////////////////////////////////////////////////////////////////////

Camera* GraphicalEntitiesFactory::createCamera(const std::string& name)
{
   return new Camera(name, m_renderer);
}

///////////////////////////////////////////////////////////////////////////////

Light* GraphicalEntitiesFactory::createLight(const std::string& name)
{
   return new Light(name);
}

///////////////////////////////////////////////////////////////////////////////

LightReflectingProperties* GraphicalEntitiesFactory::createLightReflectingProperties()
{
   return new LightReflectingProperties();
}

///////////////////////////////////////////////////////////////////////////////

Texture* GraphicalEntitiesFactory::createTexture(const std::string& name)
{
   Texture* texture = NULL;
   if (name.length() == 0)
   {
      texture = createEmptyTexture();
   }
   else
   {
      texture = loadTexture(m_texturesPath, name);
   }

   return texture;
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
      texture = createTexture(textureName);
      m_textures.add(texture);
   }

   return new MaterialStage(*texture,
                            new MaterialOperation(colorOp, colorArg1, colorArg2),
                            new MaterialOperation(alphaOp, alphaArg1, alphaArg2),
                            new CoordinatesOperation(coordsOperation));
}

///////////////////////////////////////////////////////////////////////////////

MaterialStage* GraphicalEntitiesFactory::createMaterialStage(
                  Texture& texture,
                  MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                  MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2,
                  CoordsOpCode coordsOperation)
{
   return new MaterialStage(texture,
                            new MaterialOperation(colorOp, colorArg1, colorArg2),
                            new MaterialOperation(alphaOp, alphaArg1, alphaArg2),
                            new CoordinatesOperation(coordsOperation));
}

///////////////////////////////////////////////////////////////////////////////

Material* GraphicalEntitiesFactory::createMaterial(
                                          const std::string& materialName,
                                          LightReflectingProperties* lrp)
{
   Material* mat = new Material(materialName);
   mat->setLightReflectingProperties(lrp);
   return mat;
}

///////////////////////////////////////////////////////////////////////////////
