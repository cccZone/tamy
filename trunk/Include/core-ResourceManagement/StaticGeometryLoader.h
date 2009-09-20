#pragma once

/// @file   core-ResourceManagement\StaticEntityLoader
/// @brief  static mesh loading utility

#include "core-ResourceManagement\MeshDefinition.h"
#include "core-Renderer\Skeleton.h"
#include "core\ResourceStorage.h"
#include <string>
#include <vector>
#include <map>

// ISSUE: mozna by rozdzielic wgrywanie materialow z plikow 
//        od wgrywania geometrii. Co prawda podwojne parsowanie,
//        ale za to wieksza swoboda - do przemyslenia

///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
class Material;
class GraphicalDataSource;
class Material;
class StaticGeometry;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class allows to load meshes from various sources.
 * The source of the mesh is implementation-dependent, however
 * the rules how to create a mesh are defined here.
 * This ensures that all the meshes will be created in a way
 * the engine can deal with them, and where do they come from -
 * that can vary.
 */
class StaticGeometryLoader
{
private:
   GraphicalEntitiesFactory& m_factory;

   typedef std::map<unsigned int, MeshDefinition> MeshPerMaterialMap;

public:
   /**
    * Constructor.
    *
    * @param factory             factory that will create
    *                            graphical representations of object 
    *                            a mesh is comprised of
    */
   StaticGeometryLoader(GraphicalEntitiesFactory& factory);

   /**
    * Creates a mesh based on the data received from the implementation 
    * (via the parseMesh method).
    *
    * @throw std::logic_error if no mesh was created for some reason.
    *
    * @param materialsStorage    this is where the materials loaded from
    *                            the file will be stored
    * @param source              the source of the geometry we'll be parsing
    * @param geometryStorage     this is where the parsed meshes will be output
    */
   void load(ResourceStorage<Material>& materialsStorage,
             GraphicalDataSource& source,
             StaticGeometry& geometryStorage);

private:
   Material& createMaterial(MaterialDefinition& material,
                            ResourceStorage<Material>& materialsStorage);

   void extractSubMeshes(const std::string& extractedMaterialName,
                         MeshDefinition& inMesh, 
                         std::vector<LitVertex>& outVertices,
                         std::vector<Face<USHORT> >& outFaces);
};

///////////////////////////////////////////////////////////////////////////////
