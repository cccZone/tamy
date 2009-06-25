#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\FileGraphicalEntityLoader.h"
#include <string>
#include <stdexcept>
#include <list>


//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<GraphicalEntityLoader> : public AbstractResourceFactory
{
private:
   std::string m_meshesDir;
   std::list<FileGraphicalEntityLoader*> m_graphicalEntitiesLoaders;

public:
   Factory(const std::string& meshesDir) : m_meshesDir(meshesDir) {}

   ~Factory()
   {
      for (std::list<FileGraphicalEntityLoader*>::iterator it = m_graphicalEntitiesLoaders.begin();
         it != m_graphicalEntitiesLoaders.end(); ++it)
      {
         delete *it;
      }
      m_graphicalEntitiesLoaders.clear();
   }

   void add(FileGraphicalEntityLoader* loader)
   {
      if (loader == NULL)
      {
         throw std::invalid_argument(
            std::string("NULL pointer instead a FileGraphicalEntityLoader instance"));
      }

      m_graphicalEntitiesLoaders.push_back(loader);
      loader->setMeshesDir(m_meshesDir);
   }

   GraphicalEntityLoader& operator()(const std::string& fileName)
   {
      for (std::list<FileGraphicalEntityLoader*>::iterator it = m_graphicalEntitiesLoaders.begin();
         it != m_graphicalEntitiesLoaders.end(); ++it)
      {
         FileGraphicalEntityLoader& loader = **it;
         if (loader.canHandleFile(fileName))
         {
            return loader;
         }
      }

      throw std::out_of_range(
         std::string("There's no loader capable of loading the mesh file ") + fileName);
   }
};

//////////////////////////////////////////////////////////////////////////////
