#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"


//////////////////////////////////////////////////////////////////////////////

class LightReflectingProperties;
class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<LightReflectingProperties> : public AbstractResourceFactory
{
public:
   virtual ~Factory() {}

   virtual LightReflectingProperties* operator()() = 0;
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class LightReflectingPropertiesFactory : public Factory<LightReflectingProperties>
{
protected:
   ResourceManager& m_resMgr;

public:
   LightReflectingPropertiesFactory(ResourceManager& resMgr) : m_resMgr(resMgr) {}

   LightReflectingProperties* operator()();
};

//////////////////////////////////////////////////////////////////////////////
