#pragma once

#include "GraphicalEntityLoader.h"
#include "Face.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class iwfMesh;
class iwfTexture;
class iwfMaterial;
class iwfSurface;

///////////////////////////////////////////////////////////////////////////////

class IWFMeshLoader : public GraphicalEntityLoader
{
private:
   iwfMesh* m_fileMesh;
   const std::vector<iwfTexture*>& m_textures;
   const std::vector<iwfMaterial*>& m_materials;

public:
   IWFMeshLoader(iwfMesh* fileMesh, 
                 const std::vector<iwfTexture*>& textures, 
                 const std::vector<iwfMaterial*>& materials);

protected:
   void parseMesh(MeshDefinition& mesh, 
                  AnimationDefinition& animation, 
                  const std::string& name);

private:
   unsigned int getMaterialDefinition(iwfTexture* texture, 
                                      iwfMaterial* material, 
                                      std::vector<MaterialDefinition>& materials);

   void addSurface(iwfSurface* surface, USHORT vertexStart, 
                   std::list<Face<USHORT> >& faces, unsigned int matIdx);
};

///////////////////////////////////////////////////////////////////////////////
