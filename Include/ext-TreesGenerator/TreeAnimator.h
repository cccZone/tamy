#pragma once

#include <string>
#include <d3dx9.h>
#include <list>


///////////////////////////////////////////////////////////////////////////////

struct MeshDefinition;
struct AnimationDefinition;
struct BoneAnimDefinition;

///////////////////////////////////////////////////////////////////////////////

class TreeAnimator
{
private:
   struct BoneCandidate
   {
      std::string name;
      D3DXMATRIX localMtx;
      unsigned int height;

      BoneCandidate(const std::string& _name,
                    const D3DXMATRIX& _localMtx,
                    unsigned int _height)
            : name(_name), localMtx(_localMtx), height(_height)
      {}
   };

public:
   void operator()(const MeshDefinition& mesh, 
                   const D3DXVECTOR3& windDirection,
                   float windStrength,
                   float animationLengthInSecs,
                   AnimationDefinition& outAnimation);

private:
   void extractSkeletonInfo(const MeshDefinition& mesh, 
                            std::list<BoneCandidate>& outBones,
                            unsigned int& outSkeletonHeight) const;

   void animateBone(const BoneCandidate& bone, 
                    unsigned int skeletonHeight, 
                    float animationLengthInSecs,
                    float ticksPerSec,
                    unsigned char keysCount, 
                    float windStrength,
                    const D3DXVECTOR3& windDirection,
                    BoneAnimDefinition& outAnim);
};

///////////////////////////////////////////////////////////////////////////////
