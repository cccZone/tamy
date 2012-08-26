/// @file   ml-FBX/FBXSceneLoader.h
/// @brief  an FBX scene loader
#pragma once

#include <string>
#include "core/ResourceImporter.h"


///////////////////////////////////////////////////////////////////////////////

class Model;

///////////////////////////////////////////////////////////////////////////////

class FBXSceneLoader : public TResourceImporter< Model >
{
public:
   /**
    * Constructor.
    */
   FBXSceneLoader( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer );

   // -------------------------------------------------------------------------
   // ResourceImporter implementation
   // -------------------------------------------------------------------------
  void import( Model& scene );
};

///////////////////////////////////////////////////////////////////////////////
