/// @file   ml-BVH/BVHLoader.h
/// @brief  a BVH file loader
#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class ResourcesManager;
class IProgressObserver;
class Model;
class SimpleFileParser;
class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

class BVHLoader
{
private:
   const Filesystem&                               m_fs;
   std::string                                     m_fileName;
   ResourcesManager&                               m_rm;
   IProgressObserver&                              m_observer;

public:
   /**
    * Constructor.
    *
    * @param fs            filesystem
    * @param fileName      name of the loaded file
    * @param rm            resources manager that manages the scene resources
    * @param observer      loading progress observer
    */
   BVHLoader( const Filesystem& fs, const std::string& fileName, ResourcesManager& rm, IProgressObserver& observer );
   ~BVHLoader();

   /**
    * The method will load a scene from a BVH file.
    *
    * @param scene         a model to which the scene should be uploaded
    */
   void load( Model& scene );
};

///////////////////////////////////////////////////////////////////////////////
