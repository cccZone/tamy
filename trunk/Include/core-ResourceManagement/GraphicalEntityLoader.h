#pragma once

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
class RenderingTechnique;

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
private:
   GraphicalEntitiesFactory& m_factory;
   ResourceStorage<RenderingTechnique>& m_renderingTechniquesStorage;

public:
   GraphicalEntityLoader(GraphicalEntitiesFactory& factory,
                         ResourceStorage<RenderingTechnique>& renderingTechniquesStorage);

   /**
    * Creates a mesh based on the data received from the implementation 
    * (via the parseMesh method)
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
