#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

ResourcesManager::ResourcesManager()
: m_filesystem(new Filesystem())
, m_resourcesCount(0)
{
   associate<ResourcesManager> (*this);
}

///////////////////////////////////////////////////////////////////////////////

ResourcesManager::~ResourcesManager()
{
   delete m_filesystem; m_filesystem = NULL;

   for (ResourcesMap::iterator it = m_resources.begin();
        it != m_resources.end(); ++it)
   {
      ResourcesVec& resVec = it->second;
      unsigned int count = resVec.size();
      for (unsigned int i = 0; i < count; ++i)
      {
         delete resVec[i];
      }
   }
   m_resources.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ResourcesManager::setFilesystem(Filesystem* filesystem)
{
   if (filesystem == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a Filesystem instance");
   }

   delete m_filesystem;
   m_filesystem = filesystem;
}

///////////////////////////////////////////////////////////////////////////////
