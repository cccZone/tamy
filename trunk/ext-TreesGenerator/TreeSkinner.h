#pragma once

#include <string>
#include <d3dx9.h>
#include <deque>
#include <list>
#include <map>

///////////////////////////////////////////////////////////////////////////////

struct TreeSegment;
struct MeshDefinition;
struct MaterialDefinition;

///////////////////////////////////////////////////////////////////////////////

/**
 * The class creates a mesh around the passed tree structure definition
 */
class TreeSkinner
{
private:
   struct TreeBoneDefinition
   {
      const TreeSegment& treeSeg;
      MeshDefinition& mesh;
      const TreeSegment* parentTreeSeg;
      int branchIdx;
      int boneIdx;

      TreeBoneDefinition(const TreeSegment& _treeSeg,
                         MeshDefinition& _mesh,
                         const TreeSegment* _parentTreeSeg,
                         int _branchIdx,
                         int _boneIdx)
            : treeSeg(_treeSeg),
            mesh(_mesh),
            parentTreeSeg(_parentTreeSeg),
            branchIdx(_branchIdx),
            boneIdx(_boneIdx)
      {}
   };

private:
   const TreeSegment& m_treeRoot;
   unsigned int m_treeHeight;

public:
   TreeSkinner(const TreeSegment& treeRoot);

   MeshDefinition* operator()(const std::string& entityName, 
                              unsigned int skinningResolution,
                              unsigned int bonesResolution,
                              const MaterialDefinition& requestedMaterial);
private:
   unsigned int getTreeHeight(const TreeSegment& treeRoot) const;

   void createSkeleton(unsigned int bonesResolution,
                       std::list<TreeBoneDefinition>& skeleton,
                       std::list<TreeBoneDefinition>& skins) const;

   bool isTimeForBranch(const TreeSegment& treeSegment, unsigned int bonesResolution) const;

   void createBone(const TreeBoneDefinition& boneDef);

   void createSkin(const TreeBoneDefinition& boneDef,
                   unsigned int skinningResolution,
                   unsigned int bonesResolution);

   D3DXMATRIX extractLocalMatrix(const TreeSegment& treeSeg, 
                                 const TreeSegment* parentSeg);

   D3DXMATRIX extractGlobalMatrix(const TreeSegment& treeSeg);

   void setMaterials(const MaterialDefinition& requestedMaterial,
                     MeshDefinition& outMesh);

   void addCylindricalVertices(const TreeSegment& treeSeg, 
                               const TreeSegment* parentTreeSeg, 
                               unsigned int skinningResolution,
                               bool swappedWeights,
                               MeshDefinition& outMesh);

   void addEndVertex(const TreeSegment& treeSeg, 
                     const TreeSegment* parentTreeSeg, 
                     bool swappedWeights,
                     MeshDefinition& outMesh);

   void addCylindricalFaces(unsigned int skinningResolution,
                            MeshDefinition& outMesh,
                            DWORD attributeID);

   void addEndFaces(unsigned int skinningResolution,
                    MeshDefinition& outMesh,
                    DWORD attributeID);

   void extractOffsetMatrices(const MeshDefinition& rootMesh,
                              std::map<std::string, D3DXMATRIX>& offsetMatrices);

   void updateMeshesWithOffsetMatrices(MeshDefinition& rootMesh,
                                       std::map<std::string, D3DXMATRIX>& offsetMatrices);
};

///////////////////////////////////////////////////////////////////////////////
