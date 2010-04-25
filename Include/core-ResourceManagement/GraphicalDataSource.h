#pragma once

/// @file   core-ResourceManagement\GraphicalDataSource.h
/// @param  mesh definition storage

#include "core-ResourceManagement\MeshDefinition.h"
#include "core-Renderer\Skeleton.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class implementation provides the definition of a mesh
 * to the caller.
 */
class GraphicalDataSource
{
public:
   virtual ~GraphicalDataSource() {}

   /**
    * The method is required to fill in the 'mesh' param with
    * the structure of a mesh from the source.
    *
    * @param name - the name of the mesh being parsed - may come in handy
    *               if the mesh comes from a file or something.
    */
   virtual void parseMesh(MeshDefinition& mesh, 
                          AnimationDefinition& animation, 
                          std::vector<MaterialDefinition>& materials,
                          const std::string& name) = 0;
};

///////////////////////////////////////////////////////////////////////////////
