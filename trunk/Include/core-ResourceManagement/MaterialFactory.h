#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"
#include "core-ResourceManagement\ResourceStorage.h"
#include "core-Renderer\Material.h"
#include <string>


//////////////////////////////////////////////////////////////////////////////

class LightReflectingProperties;
class ResourceManager;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<Material> : public AbstractResourceFactory, 
                          public Storage<Material>
{
private:
   unsigned int m_materialID;

public:
   Factory() : m_materialID(0) {}

   virtual ~Factory() {}

   /**
    * This method creates a new material from scratch
    */
   Material& operator()(const std::string& materialName, 
                        LightReflectingProperties* lrp)
   {
      Material* mat = create(materialName, m_materialID, lrp);
      ++m_materialID;
      if (add(mat) == true)
      {
         return *mat;
      }
      else
      {
         throw std::runtime_error(std::string("Material ") + materialName + " already exists");
      }
   }

protected:
   virtual Material* create(const std::string& materialName,
                            unsigned int index, 
                            LightReflectingProperties* lrp) = 0;
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class MaterialFactory : public Factory<Material>
{
protected:
   ResourceManager& m_resMgr;

public:
   MaterialFactory(ResourceManager& resMgr) : m_resMgr(resMgr) {}

protected:
   Material* create(const std::string& materialName, 
                    unsigned int index, 
                    LightReflectingProperties* lrp);
};

//////////////////////////////////////////////////////////////////////////////
