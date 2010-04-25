#ifndef _RESOURCES_MANAGER_H
#error "This file can only be included from ResourcesManager.h"
#else

#include <stdexcept>
#include "core\Filesystem.h"
#include "core\Resource.h"


///////////////////////////////////////////////////////////////////////////////

unsigned int ResourcesManager::getResourcesCount() const
{
   return m_resourcesCount;
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

template<typename RES_TYPE>
void ResourcesManager::addResource(RES_TYPE* resource, const std::string& name)
{
   if (resource == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a Resource instance");
   }

   ResourcesMap::iterator it = m_resources.find(name);
   if (it == m_resources.end())
   {
      it = m_resources.insert(std::make_pair(name, ResourcesVec())).first;
   }

   ResourcesVec& resVec = it->second;
   RES_TYPE* res = findResource<RES_TYPE> (resVec);

   if (res != NULL)
   {
      throw std::runtime_error(std::string("Resource ") + name + " of this type is already registered");
   }

   it->second.push_back(resource);
   resource->onLoaded(*this);
   ++m_resourcesCount;
}

///////////////////////////////////////////////////////////////////////////////

template<typename RES_TYPE>
RES_TYPE& ResourcesManager::create(const std::string& name)
{
   ResourcesMap::iterator it = m_resources.find(name);
   if (it == m_resources.end())
   {
      it = m_resources.insert(std::make_pair(name, ResourcesVec())).first;
   }

   ResourcesVec& resVec = it->second;
   RES_TYPE* res = findResource<RES_TYPE> (resVec);

   if (res == NULL)
   {
      // this resource hasn't been loaded yet
      try
      {
         res = new RES_TYPE(*m_filesystem, name);
      }
      catch (std::exception& ex)
      {
         throw ex;
      }
      resVec.push_back(res);
      res->onLoaded(*this);
      ++m_resourcesCount;
   }

   return *res;
}

///////////////////////////////////////////////////////////////////////////////

template<typename RES_TYPE>
RES_TYPE* ResourcesManager::findResource(ResourcesVec& resources)
{
   RES_TYPE* res = NULL;

   unsigned int count = resources.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      res = dynamic_cast<RES_TYPE*> (resources[i]);
      if (res != NULL)
      {
         break;
      }
   }

   return res;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCES_MANAGER_H
