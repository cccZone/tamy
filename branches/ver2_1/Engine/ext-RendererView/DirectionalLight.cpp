#include "ext-RendererView\DirectionalLight.h"
#include "ext-RendererView\RendererView.h"
#include "ext-RendererView\LightsScene.h"
#include "core-Scene\DirectionalLightEntity.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"


namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight(DirectionalLightEntity& entity)
: m_entity(entity)
, m_light(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

DirectionalLight::~DirectionalLight()
{
   delete m_light; m_light = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::initialize(RendererView& rendererView)
{
   if (m_light != NULL)
   {
      throw std::runtime_error("This light already has a representation");
   }

   m_light = rendererView.factory().createLight(m_entity.m_name);
   m_light->setType(Light::LT_DIRECTIONAL);
   m_light->setAmbientColor(m_entity.m_ambient);
   m_light->setDiffuseColor(m_entity.m_diffuse);
   m_light->setSpecularColor(m_entity.m_specular);
   m_light->setLocalMtx(m_entity.m_situation);

   rendererView.lights().add(*m_light);
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::deinitialize(RendererView& rendererView)
{
   if (m_light == NULL)
   {
      throw std::runtime_error("This light doesn't have a representation");
   }

   rendererView.lights().remove(*m_light);

   delete m_light; m_light = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLight::update(float timeElapsed)
{
   // TODO
}

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
