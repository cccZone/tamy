#pragma once

/// @file   core-ResourceManagement\GraphicalEntityLoader
/// @brief  dynamic mesh loading utility

#include "core-ResourceManagement\MeshDefinition.h"
#include "core-Renderer\Skeleton.h"
#include "core\ResourceStorage.h"
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
class AbstractGraphicalEntity;
class Material;
class GraphicalDataSource;
class Material;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class allows to load meshes from various sources.
 * The source of the mesh is implementation-dependent, however
 * the rules how to create a mesh are defined here.
 * This ensures that all the meshes will be created in a way
 * the engine can deal with them, and where do they come from -
 * that can vary.
 */
class GraphicalEntityLoader
{
private:
   GraphicalEntitiesFactory& m_factory;
   ResourceStorage<Material>& m_materialsStorage;

public:
   /**
    * Constructor.
    *
    * @param factory             factory that will create
    *                            graphical representations of object 
    *                            a mesh is comprised of
    * @param materialsStorage    this is where the materials created during
    *                            mesh creation will be stored.
    */
   GraphicalEntityLoader(GraphicalEntitiesFactory& factory,
                         ResourceStorage<Material>& materialsStorage);

   /**
    * Creates a mesh based on the data received from the implementation 
    * (via the parseMesh method).
    *
    * @throw std::logic_error if no mesh was created for some reason.
    */
   AbstractGraphicalEntity* load(const std::string& name, 
                                 GraphicalDataSource& source);

private:
   void ensureMeshNames(MeshDefinition& mesh);

   void createMaterials(std::vector<MaterialDefinition>& materials);

   AbstractGraphicalEntity* translateMeshToEntity(MeshDefinition& mesh);
};

///////////////////////////////////////////////////////////////////////////////
