#ifndef _IWF_SCENE_H
#define _IWF_SCENE_H

/// @file      ml-IWF\IWFScene.h
/// @brief     IWF files loader

#include "core-MVC.h"
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
class Filesystem;
class SingletonsManager;
class ResourcesManager;
struct Color;
struct MeshDefinition;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class will load a scene form an IWF file.
 */
 class IWFScene
{
private:
   const Filesystem& m_fs;
   std::string m_sceneDir;
   std::string m_fileName;

public:
   /** 
    * Constructor.
    */
   IWFScene( const Filesystem& fs, const std::string& fileName );
   ~IWFScene();

   /**
    * The method will load a scene from an IWF file.
    *
    * @param scene         a model to which the scene should be uploaded
    * @param rm            resources manager that manages the scene resources
    * @param observer      loading progress observer
    */
   template< typename TProgressObserver >
   void load( Model& scene, ResourcesManager& rm, TProgressObserver& observer );

private:
   void processEntities(iwfEntity* fileEntity);

   std::vector<std::string> extractSkyBoxTextures(UCHAR* skyBoxData) const;

   ReferenceEntity extractReference(UCHAR* referenceData) const;

   std::string getUniqueNameForMesh(const char* originalMeshName) const;

   // -------------------------------------------------------------------------
   // factory methods
   // -------------------------------------------------------------------------

   /**
    * Adds a new light to the scene.
    *
    * @param name       light name
    * @param ambient    light ambient color
    * @param diffuse    light diffuse color
    * @param specular   light specular color
    * @param lightType  type of the light (spot, point, directional)
    * @param range      how far does the light emit its rays
    * @param constAtt   constant attenuation factor
    * @param linearAtt  linear attenuation factor
    * @param fallOff    spotlight's inner-to-outer cone attenuation factor
    * @param theta      spotlight's inner cone angle
    * @param phi        spotlight's outer cone angle
    * @param situation  matrix describing the position and orientation 
    *                   of the light
    */
   virtual void addLight(const std::string& name,
                         const Color& ambient, 
                         const Color& diffuse, 
                         const Color& specular,
                         long lightType,
                         float range,
                         float constAtt, float linearAtt, float quadAtt,
                         float fallOff, float theta, float phi,
                         const D3DXMATRIX& situation);

   /** 
    * Adds a new sky box to the scene.
    * 
    * @param frontTex   front side texture
    * @param backTex    back side texture
    * @param leftTex    left side texture
    * @param rightTex   right side texture
    * @param topTex     top side texture
    * @param bottomTex  bottom side texture
    */
   virtual void addSkyBox(const std::string& frontTex,
                          const std::string& backTex,
                          const std::string& leftTex,
                          const std::string& rightTex,
                          const std::string& topTex,
                          const std::string& bottomTex);

   /**
    * Adds a new dynamic mesh to the scene.
    * 
    * @param meshFileName     name of the file that contains the mesh
    * @param situation        matrix describing the position and orientation 
    *                         of the object
    */
   virtual void addDynamicMesh(const std::string& meshFileName, 
                               const D3DXMATRIX& situation);

   /**
    * Adds a new static mesh to the scene.
    * 
    * @param scene            scene to which the data will be added
    * @param rm               resources manager that manages the scene resources
    * @param mesh             mesh data
    * @param situation        matrix describing the position and orientation 
    *                         of the mesh
    */
   virtual void addStaticGeometry(Model& scene,
                                  ResourcesManager& rm,
                                  std::vector<MeshDefinition> meshes,
                                  const D3DXMATRIX& situation);
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

#include "ml-IWF\IWFScene.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _IWF_SCENE_H
