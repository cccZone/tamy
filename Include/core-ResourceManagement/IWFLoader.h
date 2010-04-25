#pragma once

/// @file      core-ResourceManagement\IWFLoader.h
/// @brief     IWF files loader

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
class IWFScene;
class Filesystem;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class will load a scene form an IWF file.
 */
class IWFLoader
{
public:
   /**
    * Interface that gets notified about the progress in loading an IWF file.
    */
   class ProgressObserver
   {
   public:
      virtual ~ProgressObserver() {}

      /**
       * The method is called when some progress in loading is made.
       *
       * @param percentage    percentage <0, 1> of data already loaded
       */
      virtual void setProgress(float percentage) = 0;
   };

private:
   IWFScene& m_scene;
   ProgressObserver* m_defaultProgressObserver;
   ProgressObserver* m_progressObserver;

public:
   /** 
    * Constructor.
    *
    * @param scene      a storage that will be used to store
    *                   the contents of the IWF file
    */
   IWFLoader(IWFScene& scene);
   ~IWFLoader();

   /**
    * The method attaches a new progress observer.
    *
    * @param observer   progress observer instance
    */
   void attach(ProgressObserver& observer);

   /**
    * The method will load a scene from an IWF file.
    *
    * @param filesystem    filesystem from which we want to load the file
    * @param fileName      IWF file name
    */
   void load(Filesystem& filesystem, const std::string& fileName);

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
