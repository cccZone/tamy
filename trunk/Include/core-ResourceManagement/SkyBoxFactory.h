#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"


//////////////////////////////////////////////////////////////////////////////

class SkyBox;
class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<SkyBox> : public AbstractResourceFactory
{
public:
   virtual ~Factory() {}

   virtual SkyBox* operator()() = 0;
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class SkyBoxFactory : public Factory<SkyBox>
{
protected:
   ResourceManager& m_resMgr;

public:
   SkyBoxFactory(ResourceManager& resMgr) : m_resMgr(resMgr) {}

   SkyBox* operator()();
};

//////////////////////////////////////////////////////////////////////////////
