#include "ext-TreesGenerator\TreeSkinner.h"
#include "ext-TreesGenerator\TreeSegment.h"
#include "core-ResourceManagement\MeshDefinition.h"
#include <math.h>
#include <set>
#include <cassert>
#include <stdexcept>
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

TreeSkinner::TreeSkinner(const TreeSegment& treeRoot)
      : m_treeRoot(treeRoot),
      m_treeHeight(0)
{
   m_treeHeight = getTreeHeight(m_treeRoot);

   if (m_treeHeight == 0)
   {
      throw std::invalid_argument("Tree can't have 0 height");
   }
}

///////////////////////////////////////////////////////////////////////////////


MeshDefinition* TreeSkinner::operator()(const std::string& entityName, 
                                        unsigned int skinningResolution,
                                        unsigned int bonesResolution,
                                        const MaterialDefinition& requestedMaterial)
{
   if (skinningResolution < 3)
   {
      throw std::invalid_argument("Skinning resolution has to be >= 3");
   }
   if (bonesResolution == 0)
   {
      throw std::invalid_argument("Bones resolution has to be > 0");
   }

   MeshDefinition* root = NULL;

   std::list<TreeBoneDefinition> skeleton;
   std::list<TreeBoneDefinition> skins;
   createSkeleton(bonesResolution, skeleton, skins);
   if ((skeleton.size() == 0) || (skins.size() == 0))
   {
      throw std::runtime_error("Invalid tree sturcture");
   }
   else
   {
      root = &(skeleton.front().mesh);
   }

   // process the bones
   for (std::list<TreeBoneDefinition>::iterator it = skeleton.begin();
        it != skeleton.end(); ++it)
   {
      createBone(*it);
   }

   // process the skins
   for (std::list<TreeBoneDefinition>::iterator it = skins.begin();
        it != skins.end(); ++it)
   {
      createSkin(*it, skinningResolution, bonesResolution);
   }

   // create the information about the offset matrices
   std::map<std::string, D3DXMATRIX> offsetMatrices;
   extractOffsetMatrices(*root, offsetMatrices);
   updateMeshesWithOffsetMatrices(*root, offsetMatrices);

   // set the materials on the entire hierarchy of meshes
   setMaterials(requestedMaterial, *root);

   return root;
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

void TreeSkinner::createSkeleton(unsigned int bonesResolution,
                                 std::list<TreeBoneDefinition>& skeleton,
                                 std::list<TreeBoneDefinition>& skins) const
{
   std::deque<TreeBoneDefinition> segsQueue;
   segsQueue.push_back(TreeBoneDefinition(m_treeRoot,
                                          *(reinterpret_cast<MeshDefinition*> (NULL)),
                                          NULL,
                                          -1,
                                          0));

   unsigned int branchesCounter = 0;

   while (segsQueue.size() > 0)
   {
      TreeBoneDefinition& def = segsQueue.front(); 

      switch(def.treeSeg.type)
      {
      case BRANCH_START:
         {
            // create a bone mesh
            MeshDefinition* newBone = new MeshDefinition();
            MeshDefinition* parentMesh = &(def.mesh);
            if (parentMesh != NULL) 
            {
               parentMesh->children.push_back(newBone);
               newBone->parent = parentMesh;
            }
            skeleton.push_back(TreeBoneDefinition(def.treeSeg, 
                                                  *newBone, 
                                                  def.parentTreeSeg, 
                                                  branchesCounter,
                                                  0));

            // create a skin mesh
            MeshDefinition* newSkin = new MeshDefinition();
            newBone->children.push_back(newSkin);
            newSkin->parent = newBone;
            skins.push_back(TreeBoneDefinition(def.treeSeg, 
                                               *newSkin, 
                                               def.parentTreeSeg, 
                                               branchesCounter,
                                               0));

            // get the only child (hopefully) into the processing queue
            assert(def.treeSeg.children.size() == 1);
            TreeSegment* childSeg = def.treeSeg.children.back();
            segsQueue.push_back(TreeBoneDefinition(*childSeg, 
                                                   *newBone, 
                                                   &(def.treeSeg), 
                                                   branchesCounter, 
                                                   0));
            branchesCounter++;
            break;
         }

      case BRANCH_MIDDLE:
         {
            const TreeSegment* parentSeg = def.parentTreeSeg;
            MeshDefinition* parentMesh = &(def.mesh);
            unsigned int boneIdx = def.boneIdx;

            if (isTimeForBranch(def.treeSeg, bonesResolution) == true)
            {
               MeshDefinition* newBone = new MeshDefinition();
               if (parentMesh != NULL) 
               {
                  parentMesh->children.push_back(newBone);
                  newBone->parent = parentMesh;
               }

               boneIdx++;
               skeleton.push_back(TreeBoneDefinition(def.treeSeg, 
                                                     *newBone, 
                                                     def.parentTreeSeg, 
                                                     def.branchIdx, 
                                                     boneIdx));

               parentMesh = newBone;
               parentSeg = &(def.treeSeg);
            }

            const std::list<TreeSegment*>& childrenSegs = def.treeSeg.children;
            for (std::list<TreeSegment*>::const_iterator it = childrenSegs.begin();
                 it != childrenSegs.end(); ++it)
            {
               segsQueue.push_back(TreeBoneDefinition(**it, 
                                                      *parentMesh, 
                                                      parentSeg, 
                                                      def.branchIdx, 
                                                      boneIdx));
            }
            break;
         };

      case BRANCH_END:
         {
            break;
         }
      }

      segsQueue.pop_front();
   }
}

///////////////////////////////////////////////////////////////////////////////

bool TreeSkinner::isTimeForBranch(const TreeSegment& treeSegment, 
                                  unsigned int bonesResolution) const
{
   // each n-segments a bone needs to be insterted
   if ((treeSegment.segmentIdx % bonesResolution) == 0)
   {
      return true;
   }

   // if that's not the case, then we will also need a bone
   // if there's a branch originating at the segment here

   for (std::list<TreeSegment*>::const_iterator it = treeSegment.children.begin();
        it != treeSegment.children.end(); ++it)
   {
      if ((*it)->type == BRANCH_START)
      {
         return true;
      }
   }

   // ... guess it's not the time for a new bone just yet
   return false;
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::createBone(const TreeBoneDefinition& boneDef)
{
   std::stringstream boneName;
   boneName << "branch_" << boneDef.branchIdx << "_Bone_" << boneDef.boneIdx;
   boneDef.mesh.name = boneName.str();

   D3DXMatrixIdentity(&(boneDef.mesh.localMtx));
   if (boneDef.parentTreeSeg != NULL)
   {
      boneDef.mesh.localMtx = extractLocalMatrix(boneDef.treeSeg, boneDef.parentTreeSeg);
   }
}

///////////////////////////////////////////////////////////////////////////////

D3DXMATRIX TreeSkinner::extractLocalMatrix(const TreeSegment& treeSeg, 
                                           const TreeSegment* parentSeg)
{
   D3DXMATRIX currGlobalMtx = extractGlobalMatrix(treeSeg);
   if (parentSeg == NULL)
   {
      return currGlobalMtx;
   }
   else
   {
      D3DXMATRIX localMtx;
      D3DXMATRIX parentGlobalMtx = extractGlobalMatrix(*parentSeg);

      D3DXMatrixInverse(&parentGlobalMtx, NULL, &parentGlobalMtx);
      D3DXMatrixMultiply(&localMtx, &currGlobalMtx, &parentGlobalMtx);

      for (char row = 0; row < 4; row++)
      {
         for (char col = 0; col < 4; col++)
         {
            if (fabs(localMtx.m[row][col]) < 0.000001f) {localMtx.m[row][col] = 0;}
         }
      }

      return localMtx;
   }
}

///////////////////////////////////////////////////////////////////////////////

D3DXMATRIX TreeSkinner::extractGlobalMatrix(const TreeSegment& treeSeg)
{
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

   return currMtx;
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::createSkin(const TreeBoneDefinition& boneDef,
                             unsigned int skinningResolution,
                             unsigned int bonesResolution)
{
   std::stringstream boneName;
   boneName << "branch_" << boneDef.branchIdx << "_skin";
   boneDef.mesh.name = boneName.str();
   boneDef.mesh.isSkin = true;
   D3DXMatrixIdentity(&(boneDef.mesh.localMtx));
 
   const TreeSegment* branchRootSeg = &(boneDef.treeSeg);

   std::set<std::string> distinctBoneNames;

   // generate the vertices, faces and attributes
   std::deque<const TreeSegment*> segsQueue;
   segsQueue.push_back(&(boneDef.treeSeg));

   DWORD attributeID = 0;
   DWORD boneIdx = 0;
   bool swappedWeights = false;
   bool wasPreviousWithBone = false;
   std::string previousBoneName;

   while (segsQueue.size() > 0)
   {
      const TreeSegment* currSeg = segsQueue.front();
      segsQueue.pop_front();

      switch(currSeg->type)
      {
      case BRANCH_START:
         {
            addCylindricalVertices(*currSeg, branchRootSeg, skinningResolution, swappedWeights, boneDef.mesh);

            BonesInfluenceDefinition bonesForAttibute;

            std::stringstream bone1Name;
            bone1Name << "branch_" << boneDef.branchIdx << "_Bone_" << boneIdx;
            previousBoneName = bone1Name.str();

            bonesForAttibute.push_back(previousBoneName);
            boneDef.mesh.bonesInfluencingAttribute.push_back(bonesForAttibute);
            distinctBoneNames.insert(previousBoneName);

            break;
         }

      case BRANCH_MIDDLE:
         {
            bool isBoneSegment = isTimeForBranch(*currSeg, bonesResolution);

            if (isBoneSegment) 
            {
               swappedWeights = !swappedWeights;
               wasPreviousWithBone = true;
            }
            else
            {
               wasPreviousWithBone = false;
            }

            addCylindricalVertices(*currSeg, branchRootSeg, skinningResolution, swappedWeights, boneDef.mesh);


            if (isBoneSegment) 
            {
               std::stringstream newBoneName;
               boneIdx++;
               newBoneName << "branch_" << boneDef.branchIdx << "_Bone_" << boneIdx;
               distinctBoneNames.insert(newBoneName.str());

               BonesInfluenceDefinition bonesForAttribute(2);
               if (swappedWeights)
               {
                  bonesForAttribute[1] = newBoneName.str();
                  bonesForAttribute[0] = previousBoneName;
               }
               else
               {
                  bonesForAttribute[0] = newBoneName.str();
                  bonesForAttribute[1] = previousBoneName;
               }       
               boneDef.mesh.bonesInfluencingAttribute.push_back(bonesForAttribute);
               previousBoneName = newBoneName.str();

               attributeID++;
            }

            addCylindricalFaces(skinningResolution, boneDef.mesh, attributeID);

            break;  
         }

      case BRANCH_END:
         {
            if (wasPreviousWithBone) 
            {
               swappedWeights = !swappedWeights;

               BonesInfluenceDefinition bonesForAttribute;
               bonesForAttribute.push_back(previousBoneName);
               bonesForAttribute.push_back(previousBoneName);
               boneDef.mesh.bonesInfluencingAttribute.push_back(bonesForAttribute);

               attributeID++;
            }
            addEndVertex(*currSeg, branchRootSeg, swappedWeights, boneDef.mesh);
            addEndFaces(skinningResolution, boneDef.mesh, attributeID);
            break;
         }
      }

      // put children segments up for processing
      for (std::list<TreeSegment*>::const_iterator it = currSeg->children.begin();
           it != currSeg->children.end(); ++it)
      {
         if ((*it)->type == BRANCH_START) continue; // we're not interested in branches this time

         segsQueue.push_back(*it);
      }
   }

   // update the list of skin bones definitions
   D3DXMATRIX identityMtx; D3DXMatrixIdentity(&identityMtx);
   for (std::set<std::string>::iterator it = distinctBoneNames.begin();
        it != distinctBoneNames.end(); ++it)
   {
      boneDef.mesh.skinBones.push_back(SkinBoneDefinition(*it, identityMtx));
   }
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
         currMesh->materials.push_back(requestedMaterial.matName);
      }
      
      for (std::list<MeshDefinition*>::const_iterator it = currMesh->children.begin();
           it != currMesh->children.end(); ++it)
      {
         meshesQueue.push_back(*it);
      }
   }
}


///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::addCylindricalVertices(const TreeSegment& treeSeg, 
                                         const TreeSegment* parentTreeSeg, 
                                         unsigned int skinningResolution,
                                         bool swappedWeights,
                                         MeshDefinition& outMesh)
{
   D3DXMATRIX thisSegMtx = extractLocalMatrix(treeSeg, parentTreeSeg);

   float width = treeSeg.width / 2.f;
   float angFact = (float)(2.f * D3DX_PI);
   float w1 = swappedWeights ? 0.f : 1.f;
   float w2 = swappedWeights ? 1.f : 0.f;

   for (unsigned int i = 0; i <= skinningResolution; ++i)
   {
      float part = (float)i / (float)skinningResolution;
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

      outMesh.vertices.push_back(LitVertex::skinnedOneTex(pos.x, pos.y, pos.z, 
                                                          w1, w2, 0, 
                                                          norm.x, norm.y, norm.z,
                                                          u, v));
   }
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::addEndVertex(const TreeSegment& treeSeg, 
                               const TreeSegment* parentTreeSeg,  
                               bool swappedWeights,
                               MeshDefinition& outMesh)
{
   D3DXMATRIX thisSegMtx = extractLocalMatrix(treeSeg, parentTreeSeg);

   D3DXVECTOR3 pos(thisSegMtx._41, thisSegMtx._42, thisSegMtx._43);
   D3DXVECTOR3 norm(thisSegMtx._21, thisSegMtx._22, thisSegMtx._23);
   D3DXVec3Normalize(&norm, &norm);

   float v = (float)(treeSeg.segmentIdx) / (float)m_treeHeight;

   float w1 = swappedWeights ? 0.f : 1.f;
   float w2 = swappedWeights ? 1.f : 0.f;

   if (fabs(pos.x) < 0.00001f) pos.x = 0;
   if (fabs(pos.y) < 0.00001f) pos.y = 0;
   if (fabs(pos.z) < 0.00001f) pos.z = 0;
   if (fabs(norm.x) < 0.00001f) norm.x = 0;
   if (fabs(norm.y) < 0.00001f) norm.y = 0;
   if (fabs(norm.z) < 0.00001f) norm.z = 0;

   outMesh.vertices.push_back(LitVertex::skinnedOneTex(pos.x, pos.y, pos.z,
                                                       w1, w2, 0, 
                                                       norm.x, norm.y, norm.z,
                                                       0.5f, v));
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::addCylindricalFaces(unsigned int skinningResolution,
                                      MeshDefinition& outMesh,
                                      DWORD attributeID)
{
   // add faces
   long startVertexIdx = outMesh.vertices.size() - (skinningResolution + 1)*2;
   assert(startVertexIdx >= 0);

   for (unsigned int i = 0; i < skinningResolution; ++i)
   {
      outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx + i, 
                                            (USHORT)startVertexIdx + skinningResolution + 1 + i, 
                                            (USHORT)startVertexIdx + i + 1, 
                                            attributeID));
      outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx + i + 1, 
                                            (USHORT)startVertexIdx + skinningResolution + 1 + i, 
                                            (USHORT)startVertexIdx + skinningResolution + 2 + i, 
                                            attributeID));
   }
   outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx + skinningResolution, 
                                         (USHORT)startVertexIdx + 2*skinningResolution + 1, 
                                         (USHORT)startVertexIdx, 
                                         attributeID));
   outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx, 
                                         (USHORT)startVertexIdx + 2*skinningResolution + 1, 
                                         (USHORT)startVertexIdx + skinningResolution + 1, 
                                         attributeID));
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::addEndFaces(unsigned int skinningResolution,
                              MeshDefinition& outMesh,
                              DWORD attributeID)
{
   unsigned int lastVertexIdx = outMesh.vertices.size() - 1;
   long startVertexIdx = lastVertexIdx - 1 - skinningResolution;
   assert(startVertexIdx >= 0);
   for (unsigned int i = 0; i < skinningResolution; ++i)
   {
      outMesh.faces.push_back(Face<USHORT> ((USHORT)startVertexIdx + i, 
                                             lastVertexIdx, 
                                             (USHORT)startVertexIdx + i + 1, 
                                             attributeID));
   }
   outMesh.faces.push_back(Face<USHORT> (lastVertexIdx - 1, 
                                         lastVertexIdx, 
                                         (USHORT)startVertexIdx, 
                                         attributeID));
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::extractOffsetMatrices(const MeshDefinition& rootMesh,
                                        std::map<std::string, D3DXMATRIX>& offsetMatrices)
{
   D3DXMATRIX identityMtx;
   D3DXMatrixIdentity(&identityMtx);

   std::deque<std::pair<const MeshDefinition*, D3DXMATRIX> > segsQueue;
   segsQueue.push_back(std::make_pair(&rootMesh, identityMtx));

   while(segsQueue.size() > 0)
   {
      const MeshDefinition* currSeg = segsQueue.front().first;
      const D3DXMATRIX& parentGlobalMtx = segsQueue.front().second;
      segsQueue.pop_front();

      bool isBranchStart = false;
      for (std::list<MeshDefinition*>::const_iterator it = currSeg->children.begin();
           it != currSeg->children.end(); ++it)
      {
         if ((*it)->isSkin == true)
         {
            isBranchStart = true;
            break;
         }
      }
   
      D3DXMATRIX thisSegGlobalMtx;
      if (isBranchStart == false)
      {
         thisSegGlobalMtx = currSeg->localMtx;
         D3DXMatrixMultiply(&thisSegGlobalMtx, &thisSegGlobalMtx, &parentGlobalMtx);
      }
      else
      {
         thisSegGlobalMtx = identityMtx;
      }

      D3DXMATRIX offsetMtx;
      D3DXMatrixInverse(&offsetMtx, NULL, &thisSegGlobalMtx);
      offsetMatrices.insert(std::make_pair(currSeg->name, offsetMtx));

      for (std::list<MeshDefinition*>::const_iterator it = currSeg->children.begin();
           it != currSeg->children.end(); ++it)
      {
         segsQueue.push_back(std::make_pair(*it, thisSegGlobalMtx));
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TreeSkinner::updateMeshesWithOffsetMatrices(
                             MeshDefinition& rootMesh,
                             std::map<std::string, D3DXMATRIX>& offsetMatrices)
{
   std::deque<MeshDefinition*> segsQueue;
   segsQueue.push_back(&rootMesh);

   while(segsQueue.size() > 0)
   {
      MeshDefinition* currSeg = segsQueue.front();
      segsQueue.pop_front();

      if (currSeg->isSkin == true)
      {
         for (UINT boneIdx = 0; boneIdx < currSeg->skinBones.size(); ++boneIdx)
         {
            SkinBoneDefinition& boneDef = currSeg->skinBones.at(boneIdx);

            std::map<std::string, D3DXMATRIX>::iterator it = offsetMatrices.find(boneDef.name);
            assert(it != offsetMatrices.end());

            boneDef.boneOffset = it->second;
         }
      }

      for (std::list<MeshDefinition*>::const_iterator it = currSeg->children.begin();
           it != currSeg->children.end(); ++it)
      {
         segsQueue.push_back(*it);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
