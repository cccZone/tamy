#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"
#include <string>


//////////////////////////////////////////////////////////////////////////////

class Light;
class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<Light> : public AbstractResourceFactory
{
public:
   virtual ~Factory() {}

   virtual Light* operator()(const std::string& name) = 0;
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class LightFactory : public Factory<Light>
{
protected:
   ResourceManager& m_resMgr;

public:
   LightFactory(ResourceManager& resMgr) : m_resMgr(resMgr) {}

   Light* operator()(const std::string& name);
};

//////////////////////////////////////////////////////////////////////////////