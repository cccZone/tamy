#include "ext-RendererView\SpotLight.h"
#include "ext-RendererView\RendererView.h"
#include "ext-RendererView\LightsScene.h"
#include "core-Scene\SpotLightEntity.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"


namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

SpotLight::SpotLight(SpotLightEntity& entity)
: m_entity(entity)
, m_light(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

SpotLight::~SpotLight()
{
   delete m_light; m_light = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SpotLight::initialize(RendererView& rendererView)
{
   if (m_light != NULL)
   {
      throw std::runtime_error("This light already has a representation");
   }

   m_light = rendererView.factory().createLight(m_entity.m_name);
   m_light->setType(Light::LT_SPOT);
   m_light->setAmbientColor(m_entity.m_ambient);
   m_light->setDiffuseColor(m_entity.m_diffuse);
   m_light->setSpecularColor(m_entity.m_specular);
   m_light->setRange(m_entity.m_range);
   m_light->setAttenuation(m_entity.m_constAtt, 
      m_entity.m_linearAtt, 
      m_entity.m_quadAtt);
   m_light->setSpotlightFeatures(m_entity.m_fallOff, 
                                 m_entity.m_theta, 
                                 m_entity.m_phi);
   m_light->setLocalMtx(m_entity.m_situation);

   rendererView.lights().add(*m_light);
}

///////////////////////////////////////////////////////////////////////////////

void SpotLight::deinitialize(RendererView& rendererView)
{
   if (m_light == NULL)
   {
      throw std::runtime_error("This light doesn't have a representation");
   }

   rendererView.lights().remove(*m_light);

   delete m_light; m_light = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SpotLight::update(float timeElapsed)
{
   // TODO
}

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
