#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Camera.h"
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

SkyBox* GraphicalEntitiesFactory::createSkyBox(Texture& front, Texture& back,
                                               Texture& left, Texture& right,
                                               Texture& top, Texture& bottom)
{
   SkyBox* skyBox = createSkyBox();
   skyBox->setTexture(SBS_FRONT, front);
   skyBox->setTexture(SBS_BACK, back);
   skyBox->setTexture(SBS_LEFT, left);
   skyBox->setTexture(SBS_RIGHT, right);
   skyBox->setTexture(SBS_TOP, top);
   skyBox->setTexture(SBS_BOTTOM, bottom);

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
      texture = createTexture(textureName);
      m_textures.add(texture);
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
   Material* mat = createMaterialImpl(materialName, 
                                      m_renderer.getRenderingTargetsPolicy(), 
                                      lrp);
   return mat;
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEffect* GraphicalEntitiesFactory::createEffect(const std::string& name, 
                                                        bool isTransparent,
                                                        EffectDataSource* dataSource)
{
   GraphicalEffect* effect = createEffectImpl(name, 
                                    m_renderer.getRenderingTargetsPolicy(), 
                                    isTransparent, 
                                    dataSource);
   return effect;
}

///////////////////////////////////////////////////////////////////////////////
