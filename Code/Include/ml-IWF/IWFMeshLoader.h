#pragma once

#include "core-Renderer\Face.h"
#include "core\Array.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class iwfMesh;
class iwfTexture;
class iwfMaterial;
class iwfSurface;
struct MeshDefinition;
struct MaterialDefinition;

///////////////////////////////////////////////////////////////////////////////

class IWFMeshLoader
{
private:
   iwfMesh* m_fileMesh;
   const std::vector<iwfTexture*>& m_textures;
   const std::vector<iwfMaterial*>& m_materials;

public:
   IWFMeshLoader(iwfMesh* fileMesh, 
                 const std::vector<iwfTexture*>& textures, 
                 const std::vector<iwfMaterial*>& materials);

   void parseMesh( Array<MeshDefinition>& meshes, const std::string& name );

private:
   MeshDefinition* selectMeshByMaterial( iwfTexture* texture, iwfMaterial* material, Array< MeshDefinition >& meshes );

   void addSurface( iwfSurface* surface, USHORT vertexStart, std::vector<Face >& faces );
};

///////////////////////////////////////////////////////////////////////////////
