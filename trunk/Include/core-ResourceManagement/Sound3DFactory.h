#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"
#include <string>


//////////////////////////////////////////////////////////////////////////////

class Sound3D;
class Sound;
class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<Sound3D> : public AbstractResourceFactory
{
public:
   virtual ~Factory() {}

   virtual Sound3D* operator()(const std::string& name, 
                               bool dynamic,
                               Sound& sound, 
                               float hearingRadius) = 0;
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class Sound3DFactory : public Factory<Sound3D>
{
protected:
   ResourceManager& m_resMgr;

public:
   Sound3DFactory(ResourceManager& resMgr) : m_resMgr(resMgr) {}

   Sound3D* operator()(const std::string& name, 
                       bool dynamic,
                       Sound& sound, 
                       float hearingRadius);
};

//////////////////////////////////////////////////////////////////////////////
