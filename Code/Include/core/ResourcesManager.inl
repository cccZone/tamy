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

Filesystem& ResourcesManager::getFilesystem()
{
   return *m_filesystem;
}

///////////////////////////////////////////////////////////////////////////////

const Filesystem& ResourcesManager::getFilesystem() const
{
   return *m_filesystem;
}


///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
