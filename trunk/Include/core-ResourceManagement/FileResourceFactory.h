#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"
#include <string>
#include <map>


//////////////////////////////////////////////////////////////////////////////

template<typename Resource>
class FileResourceFactory
{
private:
   std::map<std::string, Resource*> m_storage;

public:
   virtual ~FileResourceFactory() 
   {
      for (typename std::map<std::string, Resource*>::iterator it = m_storage.begin();
          it != m_storage.end(); ++it)
      {
         delete it->second;
      }
      m_storage.clear();
   }

   Resource& operator()(const std::string& fileName)
   {
      std::map<std::string, Resource*>::iterator it = m_storage.find(fileName);
      Resource* res = NULL;
      if (it == m_storage.end())
      {
         res = loadResource(fileName);
         m_storage.insert(std::make_pair(fileName, res));
      }
      else
      {
         res = it->second;
      }

      return *res;
   }

protected:
   virtual Resource* loadResource(const std::string& fileName) = 0;
};

//////////////////////////////////////////////////////////////////////////////
