#include "XFileGraphicalEntityLoader.h"
#include "LitVertex.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

XFileGraphicalEntityLoader::XFileGraphicalEntityLoader(IDirect3DDevice9& d3Device,
                                                       const std::string& meshDirPath)
      : m_d3Device(d3Device),
      m_meshDirPath(meshDirPath)
{
}

///////////////////////////////////////////////////////////////////////////////

bool XFileGraphicalEntityLoader::canHandleFile(const std::string& fileName) const
{
   std::string ext = getExtension(fileName);
   return ((ext == "x") || (ext == "X"));
}

///////////////////////////////////////////////////////////////////////////////

void XFileGraphicalEntityLoader::parseMesh(MeshDefinition& mesh, 
                                           AnimationDefinition& animation,
                                           const std::string& name)
{
   D3DXFRAME* outFrame = NULL;
   ID3DXAnimationController* outAnimationController = NULL;

   std::string fullFileName = m_meshDirPath + std::string("\\") + name;
   HRESULT hRet = D3DXLoadMeshHierarchyFromX(fullFileName.c_str(), 
                                             D3DXMESH_MANAGED, &m_d3Device, this,
                                             NULL, &outFrame, &outAnimationController);

   if (FAILED(hRet))
   {
      throw std::runtime_error(std::string("Failed to load file ") + fullFileName);
   }

   // parse the mesh definition
   parseFrames(outFrame, NULL, mesh);

   // parse the animationData
   if (outAnimationController != NULL)
   {
      parseAnimationData(outAnimationController, animation);
      outAnimationController->Release();
   }

   // cleanup
   D3DXFrameDestroy(outFrame, this);
}

////////////////////////////////////////////////////////////////////////////////

void XFileGraphicalEntityLoader::parseAnimationData(ID3DXAnimationController* animCtrl, 
                                                    AnimationDefinition& animation)
{
   HRESULT res;
   UINT animSetsCount = animCtrl->GetNumAnimationSets();
   for (UINT animSetIdx = 0; animSetIdx < animSetsCount; ++animSetIdx)
   {
      ID3DXAnimationSet* animSet = NULL;
      res = animCtrl->GetAnimationSet(animSetIdx, &animSet);
      if (FAILED(res)) {throw std::runtime_error("Can't get an animation set");}

      ID3DXKeyframedAnimationSet* kfmAnimSet;
      res = animSet->QueryInterface(IID_ID3DXKeyframedAnimationSet,(LPVOID*)&kfmAnimSet);
      animSet->Release();
      if (FAILED(res)) {continue;} /// we don't deal with non-keyframed animation sets

      animation.animSets.push_back(AnimationSetDefinition());
      AnimationSetDefinition& animSetDef = animation.animSets.back();
      parseAnimationSet(kfmAnimSet, animSetDef);
      kfmAnimSet->Release();
   }
}

////////////////////////////////////////////////////////////////////////////////

void XFileGraphicalEntityLoader::parseAnimationSet(ID3DXKeyframedAnimationSet* animSet, 
                                                   AnimationSetDefinition& animSetDef)
{

   UINT animsCount = animSet->GetNumAnimations();
   animSetDef.name = animSet->GetName();
   animSetDef.ticksPerSec = animSet->GetSourceTicksPerSecond();
   animSetDef.playbackType = animSet->GetPlaybackType();

   LPCSTR tmpBoneName = NULL;

   for (UINT animIdx = 0; animIdx < animsCount; ++ animIdx)
   {
      animSetDef.boneAnims.push_back(BoneAnimDefinition(animSet->GetNumScaleKeys(animIdx), 
                                                        animSet->GetNumRotationKeys(animIdx),
                                                        animSet->GetNumTranslationKeys(animIdx)));
      BoneAnimDefinition& boneAnimDef = animSetDef.boneAnims.back();

      // get the name of a bone this animation animates
      animSet->GetAnimationNameByIndex(animIdx, &tmpBoneName);
      boneAnimDef.boneName = tmpBoneName;

      // copy the rotation keys
      for (UINT rotKeyIdx = 0; rotKeyIdx < boneAnimDef.rotationKeysArr; ++rotKeyIdx)
      {
         animSet->GetRotationKey(animIdx, rotKeyIdx, &(boneAnimDef.rotationKeysArr[rotKeyIdx]));
      }

      // copy the translation keys
      for (UINT trnslKeyIdx = 0; trnslKeyIdx < boneAnimDef.translationKeysArr; ++trnslKeyIdx)
      {
         animSet->GetTranslationKey(animIdx, trnslKeyIdx, &(boneAnimDef.translationKeysArr[trnslKeyIdx]));
      }

      // copy the scaling keys
      for (UINT scaleKeyIdx = 0; scaleKeyIdx < boneAnimDef.scaleKeysArr; ++scaleKeyIdx)
      {
         animSet->GetScaleKey(animIdx, scaleKeyIdx, &(boneAnimDef.scaleKeysArr[scaleKeyIdx]));      
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

void XFileGraphicalEntityLoader::parseFrames(D3DXFRAME* frame, 
                                             MeshDefinition* parent, 
                                             MeshDefinition& mesh) const
{
   if (frame->pMeshContainer != NULL)
   {
      parseGeometry(frame->pMeshContainer, mesh);
   }

   mesh.localMtx = frame->TransformationMatrix;

   // proceed down the hierarchy with the parsing
   if(frame->pFrameSibling != NULL) 
   {
      if (parent == NULL)
      {
         throw std::logic_error("Parent node should not have any siblings");
      }
      else
      {
         parent->children.push_back(MeshDefinition());
         parseFrames(frame->pFrameSibling, parent, parent->children.back());
      }
   }
   if(frame->pFrameFirstChild != NULL) 
   {
      mesh.children.push_back(MeshDefinition());
      parseFrames(frame->pFrameFirstChild, &mesh, mesh.children.back());
   }
}

////////////////////////////////////////////////////////////////////////////////

void XFileGraphicalEntityLoader::parseGeometry(D3DXMESHCONTAINER* meshContainer, 
                                               MeshDefinition& mesh) const
{
   // copy the geometry
   ID3DXMesh* pMesh = meshContainer->MeshData.pMesh;
   LitVertex* pVertices = NULL;
   USHORT* pIndices = NULL;
   DWORD* pAttribs = NULL;

   HRESULT res = pMesh->LockVertexBuffer(0, (VOID**)&pVertices);
   if (FAILED(res)) {throw std::runtime_error("Can't lock vertex buffer. Invalid vertex format");}
   res = pMesh->LockIndexBuffer(0, (VOID**)&pIndices);
   if (FAILED(res)) {throw std::runtime_error("Can't lock index buffer. Invalid index format");}
   res = pMesh->LockAttributeBuffer(0, &pAttribs);
   if (FAILED(res)) {throw std::runtime_error("Can't lock attribute buffer");}

   for (DWORD i = 0; i < pMesh->GetNumFaces(); ++i, ++pAttribs)
   {
      USHORT idx1 = *pIndices; pIndices++;
      USHORT idx2 = *pIndices; pIndices++;
      USHORT idx3 = *pIndices; pIndices++;
      mesh.faces.push_back(Face<USHORT>(idx1, idx2, idx3, *pAttribs));
   }

   for (DWORD i = 0; i < pMesh->GetNumVertices(); ++i, ++pVertices)
   {
      mesh.vertices.push_back(*pVertices);
   }

   pMesh->UnlockAttributeBuffer();
   pMesh->UnlockIndexBuffer();
   pMesh->UnlockVertexBuffer();

   // copy the materials & the transformation matrix
   for (DWORD i = 0; i < meshContainer->NumMaterials; ++i)
   {
      mesh.materials.push_back(MaterialDefinition());
      MaterialDefinition& matDef = mesh.materials.back();
      D3DMATERIAL9& meshMat = meshContainer->pMaterials[i].MatD3D;

      matDef.ambient.r = meshMat.Ambient.r;
      matDef.ambient.g = meshMat.Ambient.g;
      matDef.ambient.b = meshMat.Ambient.b;
      matDef.ambient.a = meshMat.Ambient.a;

      matDef.diffuse.r = meshMat.Diffuse.r;
      matDef.diffuse.g = meshMat.Diffuse.g;
      matDef.diffuse.b = meshMat.Diffuse.b;
      matDef.diffuse.a = meshMat.Diffuse.a;

      matDef.specular.r = meshMat.Specular.r;
      matDef.specular.g = meshMat.Specular.g;
      matDef.specular.b = meshMat.Specular.b;
      matDef.specular.a = meshMat.Specular.a;

      matDef.emissive.r = meshMat.Emissive.r;
      matDef.emissive.g = meshMat.Emissive.g;
      matDef.emissive.b = meshMat.Emissive.b;
      matDef.emissive.a = meshMat.Emissive.a;

      matDef.power = meshMat.Power;

      if (meshContainer->pMaterials[i].pTextureFilename != NULL)
      {
         matDef.texName = meshContainer->pMaterials[i].pTextureFilename;
      }
   }
}

////////////////////////////////////////////////////////////////////////////////

HRESULT XFileGraphicalEntityLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
   D3DXFRAME* pNewFrame = NULL;
   *ppNewFrame = NULL;

   // Allocate a new frame
   pNewFrame = new D3DXFRAME;
   if (!pNewFrame) return E_OUTOFMEMORY;

   ZeroMemory(pNewFrame, sizeof(D3DXFRAME));
   if (Name) { pNewFrame->Name = _strdup(Name); }

   D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);

   // Pass this new pointer back out
   *ppNewFrame = pNewFrame;

   return D3D_OK;
}

////////////////////////////////////////////////////////////////////////////////

HRESULT XFileGraphicalEntityLoader::CreateMeshContainer(LPCSTR Name,
                                   CONST D3DXMESHDATA* pMeshData,
                                   CONST D3DXMATERIAL *pMaterials,
                                   CONST D3DXEFFECTINSTANCE *pEffectInstances,
                                   DWORD NumMaterials,
                                   CONST DWORD *pAdjacency,
                                   LPD3DXSKININFO pSkinInfo,
                                   LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
   DWORD fvf = LitVertex::FVF;
   DWORD flags = D3DXMESH_MANAGED;

   if (pMeshData->Type != D3DXMESHTYPE_MESH) return E_FAIL;

   ID3DXMesh* pMesh = pMeshData->pMesh;
   if (pMesh->GetFVF() == 0) return E_FAIL;

   // make sure the mesh is in the format we requested - both fvf- as well as creation flags-wise.
   {
      // Clone the mesh
      LPD3DXMESH pCloneMesh = NULL;
      IDirect3DDevice9* pDevice = NULL;
      pMesh->GetDevice(&pDevice);
      HRESULT hRet = pMesh->CloneMeshFVF(flags, 
                                         fvf,
                                         pDevice,
                                         &pCloneMesh);
      if (FAILED(hRet)) 
      {
         throw std::logic_error(std::string("Failed to clone the mesh"));
      }

      // Note: we don't release the old mesh here, because we don't own it
      pMesh = pCloneMesh;

      // Compute the normals for the new mesh if there was no normal to begin with
      if (!(pMesh->GetFVF() & D3DFVF_NORMAL)) 
      {
         D3DXComputeNormals(pMesh, pAdjacency);
      }

      // Release the device, we're done with it
      pDevice->Release();
   }

   // copy the materials
   D3DXMATERIAL* copiedMaterials = new D3DXMATERIAL[NumMaterials];
   for (DWORD i = 0; i < NumMaterials; ++i)
   {
      copiedMaterials[i].MatD3D = pMaterials[i].MatD3D;

      if( pMaterials[i].pTextureFilename != NULL)
      {
         copiedMaterials[i].pTextureFilename = _strdup(pMaterials[i].pTextureFilename);
      }
      else
      {
         copiedMaterials[i].pTextureFilename = NULL;
      }
   }

   // Allocate a mesh container structure
   D3DXMESHCONTAINER* pMeshContainer = new D3DXMESHCONTAINER;
   ZeroMemory(pMeshContainer, sizeof(D3DXMESHCONTAINER));
   pMeshContainer->Name = _strdup(Name);
   pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
   pMeshContainer->MeshData.pMesh = pMesh;
   pMeshContainer->pMaterials = copiedMaterials;
   pMeshContainer->NumMaterials = NumMaterials;

   *ppNewMeshContainer = pMeshContainer;

   return D3D_OK;
}

////////////////////////////////////////////////////////////////////////////////

HRESULT XFileGraphicalEntityLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
   // Validate Parameters
   if (!pFrameToFree) return D3D_OK;

   // Release data
   if (pFrameToFree->Name) { free(pFrameToFree->Name); } // '_tcsdup' allocated.
   delete pFrameToFree; // 'new' allocated.

   return D3D_OK;
}

////////////////////////////////////////////////////////////////////////////////

HRESULT XFileGraphicalEntityLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pContainerToFree)
{
   if (pContainerToFree->MeshData.pMesh) {pContainerToFree->MeshData.pMesh->Release();}
   if (pContainerToFree->Name) { free(pContainerToFree->Name); } // '_strdup' allocated.

   for (DWORD i = 0; i < pContainerToFree->NumMaterials; ++i)
   {
      if (pContainerToFree->pMaterials[i].pTextureFilename != NULL)
      {
         free(pContainerToFree->pMaterials[i].pTextureFilename);
      }
   }

   return D3D_OK;
}

////////////////////////////////////////////////////////////////////////////////
