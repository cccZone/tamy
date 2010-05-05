#ifndef _RESOURCES_MANAGER_H
#error "This file can only be included from ResourcesManager.h"
#else

#include <stdexcept>
#include "core\Filesystem.h"
#include "core\Resource.h"


///////////////////////////////////////////////////////////////////////////////

unsigned int ResourcesManager::getResourcesCount() const
{
   return m_resources.size();
}

///////////////////////////////////////////////////////////////////////////////

const Filesystem& ResourcesManager::getFilesystem() const
{
   return *m_filesystem;
}

///////////////////////////////////////////////////////////////////////////////

SingletonsManager& ResourcesManager::getInitializers()
{
   return m_initializers;
}

///////////////////////////////////////////////////////////////////////////////

const SingletonsManager& ResourcesManager::getInitializers() const
{
   return m_initializers;
}

///////////////////////////////////////////////////////////////////////////////

template<typename INIT_COMP>
ResourcesManager& ResourcesManager::associate(INIT_COMP& instance)
{
   m_initializers.setShared<INIT_COMP>(instance);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
