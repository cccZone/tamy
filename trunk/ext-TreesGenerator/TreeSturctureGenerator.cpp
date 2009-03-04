#include "TreeStructureGenerator.h"
#include "TreeParams.h"
#include "TreeSegment.h"
#include <d3dx9.h>
#include <deque>
#include <math.h>
#include <cassert>


//////////////////////////////////////////////////////////////////////////////

TreeStructureGenerator::TreeStructureGenerator()
{
   srand(timeGetTime());
}

//////////////////////////////////////////////////////////////////////////////

TreeSegment* TreeStructureGenerator::generate(const TreeParams& params)
{
   TreeSegment* root = createNextBranchSegment(params, NULL, true);

   std::deque<TreeSegment*> segmentsQueue;
   segmentsQueue.push_back(root);

   while (segmentsQueue.size() > 0)
   {
      TreeSegment* parent = segmentsQueue.front();
      segmentsQueue.pop_front();

      TreeSegment* segment = createNextBranchSegment(params, parent, false);
      parent->children.push_back(segment);

      switch(segment->type)
      {
      case BRANCH_START:
         {
            segmentsQueue.push_back(segment);
            break;
         }

      case BRANCH_MIDDLE:
         {
            segmentsQueue.push_back(segment);

            if (segment->segmentIdx < params.initialBranchingLevel) {break;}

            for (std::list<float>::const_iterator it = params.branchProbab.begin();
                 it != params.branchProbab.end(); ++it)
            {
               if (randomChance(*it) == true)
               {
                  TreeSegment* newBranch = createNextBranchSegment(params, segment, true);
                  segment->children.push_back(newBranch);
                  segmentsQueue.push_back(newBranch);
               }
            }

            break;
         }

      case BRANCH_END:
         {
            break;
         }
      }
   }

   return root;
}

//////////////////////////////////////////////////////////////////////////////

TreeSegment* TreeStructureGenerator::createNextBranchSegment(const TreeParams& params, 
                                                             TreeSegment* parent,
                                                             bool newBranch)
{
   TreeSegment* segment = new TreeSegment();

   if (parent == NULL)
   {
      segment->width = params.startWidth;
      segment->initWidth = params.startWidth;
      segment->segmentIdx = 0;
      segment->position = D3DXVECTOR3(0, 0, 0);
      segment->direction = params.startDirection;
   }
   else
   {
      if (newBranch)
      {
         segment->segmentIdx = parent->segmentIdx;
         segment->width = parent->width / 4.f;
         segment->initWidth = segment->width;

         segment->position = parent->position;

         variateDirection(*parent, 
                          params.minNewBranchDirVariation, 
                          params.maxNewBranchDirVariation, 
                          *segment);
      }
      else
      {
         segment->segmentIdx = parent->segmentIdx + 1;
         segment->width = parent->initWidth * (1.f - ((float)segment->segmentIdx / (float)params.maxTreeDepth));
         segment->initWidth = parent->width;

         D3DXVECTOR3 normDirVec; D3DXVec3Normalize(&normDirVec, &(parent->direction));
         segment->position = parent->position + (normDirVec * params.segmentInitialLength);

         variateDirection(*parent, 
                          params.minInternalBranchDirVariation, 
                          params.maxInternalBranchDirVariation, 
                          *segment);
      }
   }

   if (newBranch)
   {
      segment->type = BRANCH_START;
   }
   else if ((randomChance(params.abruptBranchEndProbab) == true) ||
            (segment->segmentIdx >= params.maxTreeDepth))
   {
      segment->type = BRANCH_END;
      segment->width = 0;
   }
   else
   {
      segment->type = BRANCH_MIDDLE;
   }

   return segment;
}

//////////////////////////////////////////////////////////////////////////////

void TreeStructureGenerator::variateDirection(const TreeSegment& referenceSeg, 
                                              float minVarAngle, 
                                              float maxVarAngle,
                                              TreeSegment& changedSeg)
{
   changedSeg.direction = referenceSeg.direction;
   changedSeg.rightVec = referenceSeg.rightVec;

   if ((minVarAngle == 0) && (maxVarAngle == 0)) {return;}

   float polarTheta = 360.f;
   float azimuth = (float)rand() / (float)RAND_MAX;
   float polar = (float)rand() / (float)RAND_MAX;

   azimuth = minVarAngle + ((maxVarAngle - minVarAngle) * azimuth);
   if (randomChance(50) == true) {azimuth = -azimuth;}

   polar = (polarTheta * polar) - (polarTheta / 2.0f);

   // azimuthal rotation
   D3DXMATRIX rotMtx;
   D3DXMatrixRotationAxis(&rotMtx, &(changedSeg.rightVec), D3DXToRadian(azimuth));
   D3DXVec3TransformNormal(&(changedSeg.direction), &(changedSeg.direction), &rotMtx);

   // polar rotation
   D3DXMatrixRotationAxis(&rotMtx, &(referenceSeg.direction), D3DXToRadian(polar));
   D3DXVec3TransformNormal(&(changedSeg.direction), &(changedSeg.direction), &rotMtx);
   D3DXVec3TransformNormal(&(changedSeg.rightVec), &(changedSeg.rightVec), &rotMtx);

   // make the right and dir vectors orthogonal again
   D3DXVECTOR3 tmpLookVec;
   D3DXVec3Cross(&tmpLookVec, &(changedSeg.direction), &(changedSeg.rightVec));
   D3DXVec3Cross(&(changedSeg.rightVec), &tmpLookVec, &(changedSeg.direction));
   D3DXVec3Normalize(&(changedSeg.rightVec), &(changedSeg.rightVec));
}

//////////////////////////////////////////////////////////////////////////////

bool TreeStructureGenerator::randomChance(float probab) const
{
   assert (probab >= 0);
   assert (probab <= 100);

   if (probab == 0) {return false;}

   float chance = ((float)rand() / (float)RAND_MAX) * 100.f;
   return chance <= probab;
}

//////////////////////////////////////////////////////////////////////////////
