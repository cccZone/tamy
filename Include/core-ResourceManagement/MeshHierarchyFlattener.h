#pragma once

/// @file   core-ResourceManagement\MeshHierarchyFlattener.h
/// @brief  tool for creating a list of meshes from a hierarchical mesh

#include <vector>
#include <string>
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\Face.h"


///////////////////////////////////////////////////////////////////////////////

struct MeshDefinition;

///////////////////////////////////////////////////////////////////////////////

struct FlatMesh
{
   std::vector<LitVertex> vertices;
   std::vector<Face> faces;
   std::string material;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a tool for flattening hierarchical meshes into a single 'flat' mesh.
 * Very useful for transforming meshes in order to create static geometry 
 * out of them.
 */
class MeshHierarchyFlattener
{
public:
   /**
    * This method performs the mesh flattening operation.
    *
    * @param hierarchicalMesh    mesh we want to disassemble into 
    *                            a 'flat' mesh
    * @param outMeshes           output collection of 'flat' meshes
    */
   void operator()(MeshDefinition& hierarchicalMesh, 
                   std::vector<FlatMesh*>& outMeshes);

private:
   void extractSubMeshes(const std::string& extractedMaterialName,
                         MeshDefinition& inMesh, 
                         std::vector<LitVertex>& outVertices,
                         std::vector<Face>& outFaces);
};

///////////////////////////////////////////////////////////////////////////////
