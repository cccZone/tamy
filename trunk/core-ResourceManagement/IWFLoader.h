#pragma once

#include <string>
#include <list>
#include <vector>
#include "Face.h"


///////////////////////////////////////////////////////////////////////////////

class iwfEntity;
class iwfMesh;
class iwfTexture;
class iwfMaterial;
class iwfSurface;
struct ReferenceEntity;
class ResourceManager;
class SceneManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class will load a scene form an IWF file
 */
class IWFLoader
{
private:
   ResourceManager& m_resourceManager;
   SceneManager& m_sceneManager;

public:
   IWFLoader(ResourceManager& resourceManager, SceneManager& sceneManager);

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
