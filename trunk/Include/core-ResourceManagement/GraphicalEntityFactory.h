#pragma once

#include "core-ResourceManagement\AbstractResourceFactory.h"
#include "core-ResourceManagement\ResourceFactory.h"
#include "core-ResourceManagement\ResourceStorage.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include <vector>
#include <string>


//////////////////////////////////////////////////////////////////////////////

class GraphicalEntityLoader;
struct MeshDefinition;
struct MaterialDefinition;
class ResourceManager;
class Material;
class GraphicalEntity;
class SkinnedGraphicalEntity;

//////////////////////////////////////////////////////////////////////////////

template<>
class Factory<AbstractGraphicalEntity> : public AbstractResourceFactory,
                                         public Storage<AbstractGraphicalEntity>
{
protected:
   ResourceManager& m_resMgr;

public:
   Factory(ResourceManager& resMgr) : m_resMgr(resMgr) {}

   virtual ~Factory() {}

   /**
    * This method will be used like 80% of the time.
    * It allows you to load a mesh from a file. 
    *
    * You basically need to specify the name of the file from which
    * you wish to load an entity (@param name), and the method will
    * check if an entity has already been loaded - and if so, it will return
    * the common instance.
    * If it doesn't exist, it will try loading it using one of the registered
    * GraphicalEntityLoaders.
    * If that does not succeed - it will throw an exception.
    */
   AbstractGraphicalEntity& operator()(const std::string& name);

   /**
    * The method loads an entity using the @param loader,
    * and ensures it a name specified in @param name
    */
   AbstractGraphicalEntity& operator()(const std::string& name, 
                                       GraphicalEntityLoader& loader);

   /**
    * The method creates an entity from a template specification
    * The name of the entity will be the name of the topmost
    * MeshDefinition structure in the hierarchy.
    */
   AbstractGraphicalEntity& operator()(MeshDefinition& mesh);

protected:
   virtual GraphicalEntity* createGraphicalEntity(const std::string& name,
                                                  const MeshDefinition& subMesh,
                                                  const std::vector<Material*>& registeredMaterials) = 0;

   virtual SkinnedGraphicalEntity* createSkinedGraphicalEntity(const std::string& name,
                                                               const MeshDefinition& subMesh,
                                                               const std::vector<Material*>& registeredMaterials) = 0;

private:
   void getMaterials(const std::vector<MaterialDefinition>& inMaterialDefinitions,
                     std::vector<Material*>& outRealMaterials);
};

//////////////////////////////////////////////////////////////////////////////

template<typename Impl>
class GraphicalEntityFactory : public Factory<AbstractGraphicalEntity>
{
public:
   GraphicalEntityFactory(ResourceManager& resMgr) : Factory<AbstractGraphicalEntity>(resMgr) {}

protected:
   GraphicalEntity* createGraphicalEntity(const std::string& name,
                                          const MeshDefinition& subMesh,
                                          const std::vector<Material*>& registeredMaterials);

   SkinnedGraphicalEntity* createSkinedGraphicalEntity(const std::string& name,
                                                       const MeshDefinition& subMesh,
                                                       const std::vector<Material*>& registeredMaterials);
};

//////////////////////////////////////////////////////////////////////////////
