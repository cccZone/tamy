/// @file   ml-BVH/BVHLoader.h
/// @brief  a BVH file loader
#pragma once

#include <string>
#include "core/ResourceLoader.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class ResourcesManager;
class IProgressObserver;
class Model;
class SimpleFileParser;
class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

class BVHLoader : public TResourceImporter< Model >
{
public:
   /**
    * The method will load a scene from a BVH file.
    *
    * @param fileName
    * @param rm
    * @param observer
    * @param scene         a model to which the scene should be uploaded
    */
   void load( const std::string& fileName, ResourcesManager& rm, IProgressObserver& observer, Model& scene );

   // -------------------------------------------------------------------------
   // ResourceLoader implementation
   // -------------------------------------------------------------------------
   Resource* load( ResourcesManager& rm, IProgressObserver& observer );
};

///////////////////////////////////////////////////////////////////////////////
