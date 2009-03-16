#pragma once

#include "core-ResourceManagement\FileGraphicalEntityLoader.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

struct D3DXMESHCONTAINER_DERRIVED;

///////////////////////////////////////////////////////////////////////////////

class XFileGraphicalEntityLoader : public FileGraphicalEntityLoader,
                                   public ID3DXAllocateHierarchy
{
private:
   IDirect3DDevice9& m_d3Device;
   std::string m_meshDirPath;

public:
   XFileGraphicalEntityLoader(IDirect3DDevice9& d3Device,
                              const std::string& meshDirPath);

   bool canHandleFile(const std::string& fileName) const;

   STDMETHOD(CreateFrame)         (THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);

   STDMETHOD(CreateMeshContainer) (THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData,
                                   CONST D3DXMATERIAL *pMaterials,
                                   CONST D3DXEFFECTINSTANCE *pEffectInstances,
                                   DWORD NumMaterials, CONST DWORD *pAdjacency,
                                   LPD3DXSKININFO pSkinInfo,
                                   LPD3DXMESHCONTAINER *ppNewMeshContainer);

   STDMETHOD(DestroyFrame)         (THIS_ LPD3DXFRAME pFrameToFree);

   STDMETHOD(DestroyMeshContainer) (THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

protected:
   void parseMesh(MeshDefinition& mesh, 
                  AnimationDefinition& animation,
                  const std::string& name);

   void parseFrames(D3DXFRAME* frame, 
                    MeshDefinition* parent,
                    MeshDefinition& mesh) const;
   void parseGeometry(D3DXMESHCONTAINER_DERRIVED* meshContainer, 
                      MeshDefinition& mesh) const;

   void parseAnimationData(ID3DXAnimationController* animCtrl, 
                           AnimationDefinition& animation);
   void parseAnimationSet(ID3DXKeyframedAnimationSet* animSet, 
                          AnimationSetDefinition& animSetDef);
};

///////////////////////////////////////////////////////////////////////////////

struct D3DXMESHCONTAINER_DERRIVED : public D3DXMESHCONTAINER
{
   DWORD maxFaceInfl;
   DWORD numBoneCombinations;
   LPD3DXBUFFER boneCombinationTable;

   D3DXMESHCONTAINER_DERRIVED() 
      : maxFaceInfl(0), numBoneCombinations(0), boneCombinationTable(NULL) {}
};

///////////////////////////////////////////////////////////////////////////////
