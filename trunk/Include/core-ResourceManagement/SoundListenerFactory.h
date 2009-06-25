#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"


//////////////////////////////////////////////////////////////////////////////

class SoundListener;
class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<SoundListener> : public AbstractResourceFactory
{
public:
   virtual ~Factory() {}

   virtual SoundListener* operator()() = 0;
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class SoundListenerFactory : public Factory<SoundListener>
{
protected:
   ResourceManager& m_resMgr;

public:
   SoundListenerFactory(ResourceManager& resMgr) : m_resMgr(resMgr) {}

   SoundListener* operator()();
};

//////////////////////////////////////////////////////////////////////////////
