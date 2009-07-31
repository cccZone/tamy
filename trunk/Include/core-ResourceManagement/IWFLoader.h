#pragma once

#include "core\ResourceStorage.h"
#include <string>
#include <list>
#include <vector>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class iwfEntity;
class iwfMesh;
class iwfTexture;
class iwfMaterial;
class iwfSurface;
struct ReferenceEntity;
class SceneManager;
class GraphicalEntitiesFactory;
class Material;
class GraphicalEntityLoader;
class GraphicalDataSource;
class AbstractGraphicalEntity;
class Material;
class Texture;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class will load a scene form an IWF file
 */
class IWFLoader
{
private:
   GraphicalEntitiesFactory& m_entitiesFactory;
   GraphicalEntityLoader* m_entityLoader;
   GraphicalDataSource& m_externalEntitiesSource;
   SceneManager& m_sceneManager;
   ResourceStorage<AbstractGraphicalEntity>& m_entitiesStorage;
   ResourceStorage<Material>& m_materialsStorage;

public:
   IWFLoader(GraphicalEntitiesFactory& entitiesFactory, 
             GraphicalDataSource& externalEntitiesSource,
             SceneManager& sceneManager,
             ResourceStorage<AbstractGraphicalEntity>& entitiesStorage,
             ResourceStorage<Material>& materialsStorage);
   ~IWFLoader();

   /**
    * The method will load a scene from an IWF file
    * name of which is passed in 'fileName' param,
    * and attach it to the 'rootNode'
    */
   void load(const std::string& fileName);

private:
   void processEntities(iwfEntity* fileEntity);

   std::vector<std::string> extractSkyBoxTextures(UCHAR* skyBoxData) const;

   ReferenceEntity extractReference(UCHAR* referenceData) const;

   std::string getUniqueNameForMesh(const char* originalMeshName) const;

   Texture& createSkyboxMaterial(const std::string& textureName) const;
};

///////////////////////////////////////////////////////////////////////////////

struct ReferenceEntity
{
    ULONG         referenceType;
    ULONG         flags;
    std::string  referenceName;

    ReferenceEntity() : referenceType(0), flags(0) {}
};

///////////////////////////////////////////////////////////////////////////////

#define CUSTOM_ENTITY_SKYBOX    0x202       // Skybox entity identifier
#define CUSTOM_ENTITY_REFERENCE 0x203       // Reference entity identifier
#define EXTERNAL_REFERENCE      1

///////////////////////////////////////////////////////////////////////////////
