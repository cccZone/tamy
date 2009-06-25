#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"
#include <string>


//////////////////////////////////////////////////////////////////////////////

class ParticleSystem;
class Material;
class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<ParticleSystem> : public AbstractResourceFactory
{
public:
   virtual ~Factory() {}

   virtual ParticleSystem* operator()(const std::string& name, 
                                      bool isDynamic, 
                                      Material& material,
                                      unsigned int particlesCount) = 0;
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class ParticleSystemFactory : public Factory<ParticleSystem>
{
protected:
   ResourceManager& m_resMgr;

public:
   ParticleSystemFactory(ResourceManager& resMgr) : m_resMgr(resMgr) {}

   ParticleSystem* operator()(const std::string& name, 
                              bool isDynamic, 
                              Material& material,
                              unsigned int particlesCount);
};

//////////////////////////////////////////////////////////////////////////////
