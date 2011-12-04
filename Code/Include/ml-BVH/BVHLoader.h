/// @file   ml-BVH/BVHLoader.h
/// @brief  a BVH file loader
#pragma once

#include <string>
#include "core/ResourceImporter.h"


///////////////////////////////////////////////////////////////////////////////

class Model;
class SkeletonAnimation;

///////////////////////////////////////////////////////////////////////////////

class BVHModelLoader : public TResourceImporter< Model >
{
public:
   /**
    * Constructor.
    */
   BVHModelLoader( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer );

   // -------------------------------------------------------------------------
   // ResourceImporter implementation
   // -------------------------------------------------------------------------
  void import( Model& scene );
};

///////////////////////////////////////////////////////////////////////////////

class BVHSkeletonAnimationLoader : public TResourceImporter< SkeletonAnimation >
{
public:
   /**
    * Constructor.
    */
   BVHSkeletonAnimationLoader( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer );

   // -------------------------------------------------------------------------
   // ResourceImporter implementation
   // -------------------------------------------------------------------------
  void import( SkeletonAnimation& animation );
};

///////////////////////////////////////////////////////////////////////////////
