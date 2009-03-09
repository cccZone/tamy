#pragma once

#include "MeshDefinition.h"
#include "Skeleton.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class ResourceManager;
class AbstractGraphicalEntity;
class Material;

///////////////////////////////////////////////////////////////////////////////

/**
 * This abstract class allows to load meshes from various sources.
 * The source of the mesh is implementation-dependent, however
 * the rules how to create a mesh are defined here.
 * This ensures that all the meshes will be created in a way
 * the engine can deal with them, and where do they come from -
 * that can vary.
 */
class GraphicalEntityLoader
{
public:
   virtual ~GraphicalEntityLoader() {}

   /**
    * Creates a mesh based on the data received from the implementation 
    * (via the parseMesh method), and registers it with the ResourceManager.
    *
    * @throw std::logic_error if no mesh was created for some reason.
    */
   AbstractGraphicalEntity& load(ResourceManager& resourceManager, const std::string& name);

protected:
   /**
    * The method is required to fill in the 'mesh' param with
    * the structure of a mesh from the source.
    * @param name - the name of the mesh being parsed - may come in handy
    *               if the mesh comes from a file or something.
    */
   virtual void parseMesh(MeshDefinition& mesh, 
                          AnimationDefinition& animation, 
                          const std::string& name) = 0;

private:
   void ensureMeshNames(MeshDefinition& mesh);
};

///////////////////////////////////////////////////////////////////////////////
