#include "TreeSkinner.h"
#include "TreeSegment.h"
#include "MeshDefinition.h"
#include <deque>
#include <math.h>
#include <cassert>
#include <stdexcept>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

double const M_PI = 2 * acos(0.0);

///////////////////////////////////////////////////////////////////////////////

TreeSkinner::TreeSkinner(const TreeSegment& treeRoot)
      : m_treeRoot(treeRoot),
      m_treeHeight(0),
      m_skinningResolution(0),
      m_bonesResolution(0),
      m_skinnedTreeRoot(NULL),
      m_analysisStart(true),
      m_branchIdx(0),
      m_currAnalyzedBranch(NULL)
{
   m_treeHeight = getTreeHeight(m_treeRoot);

   if (m_treeHeight == 0)
   {
      throw std::invalid_argument("Tree can't have 0 height");
   }
}

///////////////////////////////////////////////////////////////////////////////


void TreeSkinner::operator()(const std::string& entityName, 
                             unsigned int skinningResolution,
                             unsigned int bonesResolution,
                             const MaterialDefinition& requestedMaterial,
                             MeshDefinition& outMesh)
{
   if (skinningResolution < 3)
   {
      throw std::invalid_argument("Skinning resolution has to be >= 3");
   }
   if (bonesResolution == 0)
   {
      throw std::invalid_argument("Bones resolution has to be > 0");
   }

   m_skinningResolution = skinningResolution;
   m_bonesResolution = bonesResolution;
   m_analysisStart = true;
   m_skinnedTreeRoot = &outMesh;
   m_entityName = entityName;
   m_branchIdx = 0;

   createSkin();
   setMaterials(requestedMaterial, outMesh);
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::setMaterials(const MaterialDefinition& requestedMaterial,
                               MeshDefinition& outMesh)
{
   std::deque<MeshDefinition*> meshesQueue;
   meshesQueue.push_back(&outMesh);
   while(meshesQueue.size() > 0)
   {
      MeshDefinition* currMesh = meshesQueue.front();
      meshesQueue.pop_front();

      unsigned int attribsCount = currMesh->bonesInfluencingAttribute.size();
      for (unsigned int i = 0; i < attribsCount; ++i)
      {
         currMesh->materials.push_back(requestedMaterial);
      }
      
      for (std::list<MeshDefinition*>::const_iterator it = currMesh->children.begin();
           it != currMesh->children.end(); ++it)
      {
         meshesQueue.push_back(*it);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::createSkin()
{
   std::deque<SegmentToAnalyze> segsQueue;
   segsQueue.push_back(SegmentToAnalyze(NULL, &m_treeRoot));
   while(segsQueue.size() > 0)
   {
      m_currAnalyzedBranch = &(segsQueue.front());
      segsQueue.pop_front();

      const TreeSegment& treeSeg = *(m_currAnalyzedBranch->treeSeg);
      MeshDefinition& currMesh = *(m_currAnalyzedBranch->parentMesh);

      switch(treeSeg.type)
      {
      case BRANCH_START:
         {
            D3DXMATRIX localMtx = calculateLocalMtx(treeSeg, 
                                                    m_currAnalyzedBranch->m_currentBranchGlobalMtx);
            D3DXMATRIX identityMtx; D3DXMatrixIdentity(&identityMtx);

            MeshDefinition* newBranchMesh = createBranch(localMtx);
            m_currAnalyzedBranch->m_currBone = newBranchMesh;
            m_currAnalyzedBranch->m_currentBoneIdx = -1;
            defineNewBone(treeSeg, *newBranchMesh, identityMtx, identityMtx);
            addCylindricalVertices(treeSeg, *newBranchMesh);

            for (std::list<TreeSegment*>::const_iterator it = treeSeg.children.begin();
                 it != treeSeg.children.end(); ++it)
            {
               SegmentToAnalyze newSeg(*m_currAnalyzedBranch);
               newSeg.parentMesh = newBranchMesh;
               newSeg.treeSeg = *it;
               newSeg.m_currentSegGlobalMtx = localMtx;
               D3DXMatrixIdentity(&newSeg.m_globalBoneOffsetMtx);
               segsQueue.push_back(newSeg);
            }
            break;
         }
      case BRANCH_MIDDLE:
         {
            addCylindricalVertices(treeSeg, currMesh);
            addCylindricalFaces(currMesh);

            if (isTimeForNextBone(treeSeg)) 
            { 
               D3DXMATRIX localMtx = calculateLocalMtx(treeSeg,
                                                       m_currAnalyzedBranch->m_currentSegGlobalMtx);
               defineNewBone(treeSeg, currMesh, localMtx, m_currAnalyzedBranch->m_globalBoneOffsetMtx); 
            }

            for (std::list<TreeSegment*>::const_iterator it = treeSeg.children.begin();
                 it != treeSeg.children.end(); ++it)
            {
               SegmentToAnalyze newSeg(*m_currAnalyzedBranch);
               newSeg.parentMesh = &currMesh;
               newSeg.treeSeg = *it;
               segsQueue.push_back(newSeg);
            }
            break;
         }
      case BRANCH_END:
         {
            addEndVertex(treeSeg, currMesh);
            addEndFaces(currMesh);

            assert(treeSeg.children.size() == 0);

            break;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

MeshDefinition* TreeSkinner::createBranch(const D3DXMATRIX& localMtx)
{
   assert(m_skinnedTreeRoot != NULL);

   MeshDefinition* newMesh;
   if (m_analysisStart)
   {
      newMesh = m_skinnedTreeRoot;
      newMesh->name = m_entityName;
      m_analysisStart = false;
   }
   else
   {
      newMesh = new MeshDefinition();
      m_currAnalyzedBranch->parentMesh->children.push_back(newMesh);

      std::stringstream branchName;
      branchName << m_entityName << "_branch_" << m_branchIdx;
      m_branchIdx++;

      newMesh->name = branchName.str();
   }
   newMesh->isSkin = true;
   newMesh->localMtx = localMtx;
   return newMesh;
}

///////////////////////////////////////////////////////////////////////////////

bool TreeSkinner::isTimeForNextBone(const TreeSegment& treeSeg) const
{
   return ((treeSeg.segmentIdx % m_bonesResolution) == 0);
}

///////////////////////////////////////////////////////////////////////////////

D3DXMATRIX TreeSkinner::calculateLocalMtx(const TreeSegment& treeSeg, 
                                          D3DXMATRIX& parentRefMtx)
{
   // calculate the bone's local mtx
   D3DXVECTOR3 lookVec;
   D3DXVec3Cross(&lookVec, &treeSeg.direction, &treeSeg.rightVec);
   D3DXVec3Normalize(&lookVec, &lookVec);

   D3DXMATRIX currMtx; D3DXMatrixIdentity(&currMtx);
   currMtx._11 = treeSeg.rightVec.x; 
   currMtx._12 = treeSeg.rightVec.y; 
   currMtx._13 = treeSeg.rightVec.z;
   currMtx._21 = treeSeg.direction.x; 
   currMtx._22 = treeSeg.direction.y; 
   currMtx._23 = treeSeg.direction.z;
   currMtx._31 = -lookVec.x; 
   currMtx._32 = -lookVec.y; 
   currMtx._33 = -lookVec.z;
   currMtx._41 = treeSeg.position.x; 
   currMtx._42 = treeSeg.position.y; 
   currMtx._43 = treeSeg.position.z;

   D3DXMATRIX currGlobalMatrixInv;
   D3DXMATRIX localMtx;
   D3DXMatrixInverse(&currGlobalMatrixInv, NULL, &parentRefMtx);
   D3DXMatrixMultiply(&localMtx, &currMtx, &currGlobalMatrixInv);

   for (char row = 0; row < 4; row++)
   {
      for (char col = 0; col < 4; col++)
      {
         if (fabs(localMtx.m[row][col]) < 0.000001f) {localMtx.m[row][col] = 0;}
      }
   }

   D3DXMatrixMultiply(&parentRefMtx, &localMtx, &parentRefMtx);

   return localMtx;
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::defineNewBone(const TreeSegment& treeSeg,
                                MeshDefinition& outMesh,
                                const D3DXMATRIX& localMtx,
                                D3DXMATRIX& globalBoneMtx)
{
   m_currAnalyzedBranch->m_currentBoneIdx++;
      
   std::stringstream boneName;
   boneName << outMesh.name << "Bone_" << m_currAnalyzedBranch->m_currentBoneIdx;

   BonesInfluenceDefinition bonesForAttrib; bonesForAttrib.push_back(boneName.str());
   outMesh.bonesInfluencingAttribute.push_back(bonesForAttrib);

   MeshDefinition* boneMesh = new MeshDefinition();
   boneMesh->name = boneName.str();
   boneMesh->isSkin = false;
   boneMesh->localMtx = localMtx;

   m_currAnalyzedBranch->m_currBone->children.push_back(boneMesh);
   m_currAnalyzedBranch->m_currBone = boneMesh;

   // calculate the bone's offset mtx
   D3DXMATRIX boneOffsetMtx;
   D3DXMatrixMultiply(&globalBoneMtx, &(boneMesh->localMtx), &globalBoneMtx);
   D3DXMatrixInverse(&boneOffsetMtx, NULL, &globalBoneMtx);
   outMesh.skinBones.push_back(SkinBoneDefinition(boneName.str(), boneOffsetMtx));
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::addCylindricalVertices(const TreeSegment& treeSeg, MeshDefinition& outMesh)
{
   D3DXVECTOR3 rightVec = treeSeg.rightVec;
   D3DXVECTOR3 upVec = treeSeg.direction;
   D3DXVECTOR3 lookVec;
   D3DXVECTOR3 posVec = treeSeg.position;
   D3DXVec3Cross(&lookVec, &upVec, &rightVec);
   D3DXVec3Normalize(&lookVec, &lookVec);

   D3DXMATRIX thisSegMtx; D3DXMatrixIdentity(&thisSegMtx);
   thisSegMtx._11 = rightVec.x; 
   thisSegMtx._12 = rightVec.y; 
   thisSegMtx._13 = rightVec.z;
   thisSegMtx._21 = upVec.x; 
   thisSegMtx._22 = upVec.y; 
   thisSegMtx._23 = upVec.z;
   thisSegMtx._31 = -lookVec.x; 
   thisSegMtx._32 = -lookVec.y; 
   thisSegMtx._33 = -lookVec.z;
   thisSegMtx._41 = posVec.x; 
   thisSegMtx._42 = posVec.y; 
   thisSegMtx._43 = posVec.z;

   D3DXMATRIX parentLocalMatrixInv = m_currAnalyzedBranch->m_currentBranchGlobalMtx;
   D3DXMatrixInverse(&parentLocalMatrixInv, NULL, &parentLocalMatrixInv);
   D3DXMatrixMultiply(&thisSegMtx, &thisSegMtx, &parentLocalMatrixInv);

   float width = treeSeg.width / 2.f;
   float angFact = (float)(2.f * M_PI);
   for (unsigned int i = 0; i <= m_skinningResolution; ++i)
   {
      float part = (float)i / (float)m_skinningResolution;
      float x = cos(part * angFact) * width;
      float z = sin(part * angFact) * width;
      
      D3DXVECTOR3 pos(x, 0, z);
      D3DXVECTOR3 norm(x, 0, z);
      D3DXVec3TransformCoord(&pos, &pos, &thisSegMtx);
      D3DXVec3TransformNormal(&norm, &norm, &thisSegMtx);
      D3DXVec3Normalize(&norm, &norm);

      if (fabs(pos.x) < 0.00001f) pos.x = 0;
      if (fabs(pos.y) < 0.00001f) pos.y = 0;
      if (fabs(pos.z) < 0.00001f) pos.z = 0;
      if (fabs(norm.x) < 0.00001f) norm.x = 0;
      if (fabs(norm.y) < 0.00001f) norm.y = 0;
      if (fabs(norm.z) < 0.00001f) norm.z = 0;

      float u = part;
      float v = (float)(treeSeg.segmentIdx) / (float)m_treeHeight;

      outMesh.vertices.push_back(LitVertex(pos.x, pos.y, pos.z, 
                                           1, 0, 0, 
                                           norm.x, norm.y, norm.z,
                                           u, v));
   }
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::addEndVertex(const TreeSegment& treeSeg, MeshDefinition& outMesh)
{
   D3DXVECTOR3 rightVec = treeSeg.rightVec;
   D3DXVECTOR3 upVec = treeSeg.direction;
   D3DXVECTOR3 lookVec;
   D3DXVECTOR3 posVec = treeSeg.position;
   D3DXVec3Cross(&lookVec, &upVec, &rightVec);
   D3DXVec3Normalize(&lookVec, &lookVec);

   D3DXMATRIX thisSegMtx; D3DXMatrixIdentity(&thisSegMtx);
   thisSegMtx._11 = rightVec.x; 
   thisSegMtx._12 = rightVec.y; 
   thisSegMtx._13 = rightVec.z;
   thisSegMtx._21 = upVec.x; 
   thisSegMtx._22 = upVec.y; 
   thisSegMtx._23 = upVec.z;
   thisSegMtx._31 = -lookVec.x; 
   thisSegMtx._32 = -lookVec.y; 
   thisSegMtx._33 = -lookVec.z;
   thisSegMtx._41 = posVec.x; 
   thisSegMtx._42 = posVec.y; 
   thisSegMtx._43 = posVec.z;

   D3DXMATRIX parentLocalMatrixInv = m_currAnalyzedBranch->m_currentBranchGlobalMtx;
   D3DXMatrixInverse(&parentLocalMatrixInv, NULL, &parentLocalMatrixInv);
   D3DXMatrixMultiply(&thisSegMtx, &thisSegMtx, &parentLocalMatrixInv);

   D3DXVECTOR3 pos(thisSegMtx._41, thisSegMtx._42, thisSegMtx._43);
   D3DXVECTOR3 norm(thisSegMtx._21, thisSegMtx._22, thisSegMtx._23);
   D3DXVec3Normalize(&norm, &norm);

   float v = (float)(treeSeg.segmentIdx) / (float)m_treeHeight;

   if (fabs(pos.x) < 0.00001f) pos.x = 0;
   if (fabs(pos.y) < 0.00001f) pos.y = 0;
   if (fabs(pos.z) < 0.00001f) pos.z = 0;
   if (fabs(norm.x) < 0.00001f) norm.x = 0;
   if (fabs(norm.y) < 0.00001f) norm.y = 0;
   if (fabs(norm.z) < 0.00001f) norm.z = 0;

   outMesh.vertices.push_back(LitVertex(pos.x, pos.y, pos.z,
                                        1, 0, 0, 
                                        norm.x, norm.y, norm.z,
                                        0.5f, v));
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::addCylindricalFaces(MeshDefinition& outMesh)
{
   // add faces
   long startVertexIdx = outMesh.vertices.size() - (m_skinningResolution + 1)*2;
   assert(startVertexIdx >= 0);

   for (unsigned int i = 0; i < m_skinningResolution; ++i)
   {
      outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx + i, 
                                            (USHORT)startVertexIdx + m_skinningResolution + 1 + i, 
                                            (USHORT)startVertexIdx + i + 1, 
                                            m_currAnalyzedBranch->m_currentBoneIdx));
      outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx + i + 1, 
                                            (USHORT)startVertexIdx + m_skinningResolution + 1 + i, 
                                            (USHORT)startVertexIdx + m_skinningResolution + 2 + i, 
                                            m_currAnalyzedBranch->m_currentBoneIdx));
   }
   outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx + m_skinningResolution, 
                                         (USHORT)startVertexIdx + 2*m_skinningResolution + 1, 
                                         (USHORT)startVertexIdx, 
                                         m_currAnalyzedBranch->m_currentBoneIdx));
   outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx, 
                                         (USHORT)startVertexIdx + 2*m_skinningResolution + 1, 
                                         (USHORT)startVertexIdx + m_skinningResolution + 1, 
                                         m_currAnalyzedBranch->m_currentBoneIdx));
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::addEndFaces(MeshDefinition& outMesh)
{
   unsigned int lastVertexIdx = outMesh.vertices.size() - 1;
   long startVertexIdx = lastVertexIdx - 1 - m_skinningResolution;
   assert(startVertexIdx >= 0);
   for (unsigned int i = 0; i < m_skinningResolution; ++i)
   {
      outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx + i, 
                                             lastVertexIdx, 
                                             (USHORT)startVertexIdx + i + 1, 
                                             m_currAnalyzedBranch->m_currentBoneIdx));
   }
   outMesh.faces.push_back(Face<USHORT> (lastVertexIdx - 1, 
                                         lastVertexIdx, 
                                         (USHORT)startVertexIdx, 
                                         m_currAnalyzedBranch->m_currentBoneIdx));
}

///////////////////////////////////////////////////////////////////////////////

unsigned int TreeSkinner::getTreeHeight(const TreeSegment& treeRoot) const
{
   // find the height of the tree
   unsigned int treeHeight = 0;
   std::deque<const TreeSegment*> segsQueue;
   segsQueue.push_back(&treeRoot);
   while(segsQueue.size() > 0)
   {
      const TreeSegment* currSeg = segsQueue.front();
      segsQueue.pop_front();

      if (currSeg->segmentIdx > treeHeight) {treeHeight = currSeg->segmentIdx;}

      for (std::list<TreeSegment*>::const_iterator it = currSeg->children.begin();
           it != currSeg->children.end(); ++it)
      {
         segsQueue.push_back(*it);
      }
   }

   return treeHeight;
}

///////////////////////////////////////////////////////////////////////////////
