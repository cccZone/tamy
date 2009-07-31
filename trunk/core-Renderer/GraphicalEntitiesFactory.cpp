#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\SkyBox.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\PostProcessMechanism.h"
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

   return new MaterialStage(*texture,
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

GraphicalEffect* GraphicalEntitiesFactory::createEffect(const std::string& name, 
                                                        EffectDataSource* dataSource)
{
   GraphicalEffect* effect = createEffectImpl(name, 
                                    m_renderer.getRenderingTargetsPolicy(), 
                                    dataSource);
   return effect;
}

///////////////////////////////////////////////////////////////////////////////

PostProcessMechanism* 
GraphicalEntitiesFactory::createPostProcessMechanism(RenderingTargetsPolicy* policy,
                                                     GraphicalEffect& effect)
{
   PostProcessEffectRenderable* renderable = createPostProcessEffectRenderable();
   return new PostProcessMechanism(policy, effect, renderable);
}

///////////////////////////////////////////////////////////////////////////////
