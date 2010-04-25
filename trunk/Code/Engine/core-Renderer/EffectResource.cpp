#include "core-Renderer\EffectResource.h"
#include "core-Renderer\ShaderEffect.h"
#include "core\Filesystem.h"
#include "core\SingletonsManager.h"
#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

EffectResource::EffectResource(Filesystem& fs, 
      const std::string& fileName)
: m_shader(NULL)
, m_fileName(fileName)
{
   // extract the effect class name
   std::size_t extensionPos = fileName.find(".fx");
   if (extensionPos == std::string::npos)
   {
      throw std::invalid_argument("Only .fx files can be used as effect resources");
   }

   std::size_t fileNameNodePos = fileName.find_last_of("\\/") + 1;
   m_entityClassName = fileName.substr(fileNameNodePos, extensionPos - fileNameNodePos);
}

///////////////////////////////////////////////////////////////////////////////

void EffectResource::onLoaded(ResourcesManager& mgr)
{
   m_shader = &(mgr.create<Shader> (m_fileName));
}

///////////////////////////////////////////////////////////////////////////////

Effect* EffectResource::load()
{
   ShaderEffect* effect = Class::createInstance<ShaderEffect>(m_entityClassName);
   if (effect == NULL)
   {
      throw std::runtime_error(std::string("Class ") + m_entityClassName + " is not an Effect class");
   }
   effect->initialize(*m_shader);

   return effect;
}

///////////////////////////////////////////////////////////////////////////////
