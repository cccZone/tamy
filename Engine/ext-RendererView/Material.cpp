#include "ext-RendererView\Material.h"
#include "ext-RendererView\RendererView.h"
#include "core-Scene\MaterialEntity.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"


namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

Material::Material(MaterialEntity& entity)
: m_entity(entity)
{
}

///////////////////////////////////////////////////////////////////////////////

Material::~Material()
{
}

///////////////////////////////////////////////////////////////////////////////

void Material::initialize(RendererView& rendererView)
{
   if (rendererView.materials().is(m_entity.m_matName))
   {
      return;
   }

   ::Material* mat = new ::Material(m_entity.m_matName);

   LightReflectingProperties* lrp = new LightReflectingProperties();
   lrp->setAmbientColor(m_entity.m_ambient);
   lrp->setDiffuseColor(m_entity.m_diffuse);
   lrp->setSpecularColor(m_entity.m_specular);
   lrp->setEmissiveColor(m_entity.m_emissive);
   lrp->setPower(m_entity.m_power);
   mat->setLightReflectingProperties(lrp);

   Texture* texture = NULL;
   if (rendererView.textures().is(m_entity.m_texName))
   {
      texture = &(rendererView.textures().get(m_entity.m_texName));
   }
   else
   {
      texture = rendererView.factory().createTexture(m_entity.m_texName);
      rendererView.textures().add(texture);
   }

   MaterialStage* stage = new MaterialStage(*texture, 
      new MaterialOperation(m_entity.m_colorOp, m_entity.m_colorArg1, m_entity.m_colorArg2),
      new MaterialOperation(m_entity.m_alphaOp, m_entity.m_alphaArg1, m_entity.m_alphaArg2),
      new CoordinatesOperation(m_entity.m_coordsOp));
   mat->addStage(stage);

   rendererView.materials().add(mat);
}

///////////////////////////////////////////////////////////////////////////////

void Material::deinitialize(RendererView& rendererView)
{
   // TODO
}

///////////////////////////////////////////////////////////////////////////////

void Material::update(float timeElapsed)
{
   // TODO
}

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
