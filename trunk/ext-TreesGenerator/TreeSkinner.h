#pragma once

#include <string>
#include <d3dx9.h>


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
   struct SegmentToAnalyze
   {
      MeshDefinition* parentMesh;
      const TreeSegment* treeSeg;

      MeshDefinition* m_currBone;
      unsigned int m_currentBoneIdx;
      D3DXMATRIX m_globalBoneOffsetMtx;
      D3DXMATRIX m_currentSegGlobalMtx;
      D3DXMATRIX m_currentBranchGlobalMtx;

      SegmentToAnalyze(MeshDefinition* _parentMesh,
                       const TreeSegment* _treeSeg)
            : parentMesh(_parentMesh),
            treeSeg(_treeSeg),
            m_currBone(_parentMesh),
            m_currentBoneIdx(-1)
      {
         D3DXMatrixIdentity(&m_globalBoneOffsetMtx);
         D3DXMatrixIdentity(&m_currentSegGlobalMtx);
         D3DXMatrixIdentity(&m_currentBranchGlobalMtx);
      }
   };

private:
   const TreeSegment& m_treeRoot;
   unsigned int m_treeHeight;

   unsigned int m_skinningResolution;
   unsigned int m_bonesResolution;

   MeshDefinition* m_skinnedTreeRoot;
   bool m_analysisStart;
   std::string m_entityName;
   unsigned int m_branchIdx;
   SegmentToAnalyze* m_currAnalyzedBranch;

public:
   TreeSkinner(const TreeSegment& treeRoot);

   void operator()(const std::string& entityName, 
                   unsigned int skinningResolution,
                   unsigned int bonesResolution,
                   const MaterialDefinition& requestedMaterial,
                   MeshDefinition& outMesh);
private:
   unsigned int getTreeHeight(const TreeSegment& treeRoot) const;
   void createSkin();
   void setMaterials(const MaterialDefinition& requestedMaterial,
                     MeshDefinition& outMesh);

   bool isTimeForNextBone(const TreeSegment& treeSeg) const;
   MeshDefinition* createBranch(const D3DXMATRIX& localMtx);
   void defineNewBone(const TreeSegment& treeSeg, 
                      MeshDefinition& outMesh, 
                      const D3DXMATRIX& localMtx, 
                      D3DXMATRIX& globalBoneMtx);
   void addCylindricalVertices(const TreeSegment& treeSeg, MeshDefinition& outMesh);
   void addEndVertex(const TreeSegment& treeSeg, MeshDefinition& outMesh);
   void addCylindricalFaces(MeshDefinition& outMesh);
   void addEndFaces(MeshDefinition& outMesh);
   D3DXMATRIX calculateLocalMtx(const TreeSegment& treeSeg, D3DXMATRIX& parentRefMtx);
};

///////////////////////////////////////////////////////////////////////////////
