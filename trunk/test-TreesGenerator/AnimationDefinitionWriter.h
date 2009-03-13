#pragma once

#include <string>
#include "Skeleton.h"
#include "MatrixWriter.h"
#include <sstream>


///////////////////////////////////////////////////////////////////////////////

class AnimationDefinitionWriter
{
public:
   std::string operator()(const AnimationDefinition& animation) const
   {
      std::stringstream result;

      for (std::list<AnimationSetDefinition>::const_iterator it = animation.animSets.begin();
           it != animation.animSets.end(); ++it)
      {
         writeAnimationSet(*it, result);
         result << std::endl;
      }

      return result.str();
   }

private:
   void writeAnimationSet(const AnimationSetDefinition& animSet, std::stringstream& result) const
   {
      result << "Animation set '" << animSet.name << "'; playback type : ";
      switch(animSet.playbackType)
      {
      case D3DXPLAY_LOOP: result << "LOOP"; break;
      case D3DXPLAY_ONCE: result << "ONCE"; break;
      case D3DXPLAY_PINGPONG: result << "PINGPONG"; break;
      default: result << "UNKNOWN"; break;
      }

      result << "; ticks per sec : " << animSet.ticksPerSec << "; boneAnims : {" << std::endl;
      for (std::list<BoneAnimDefinition>::const_iterator it = animSet.boneAnims.begin();
           it != animSet.boneAnims.end(); ++it)
      {
         result << "[";
         writeBoneAnim(*it, result);
         result << "], " << std::endl;
      }
      result << "};";
   }

   void writeBoneAnim(const BoneAnimDefinition& boneAnim, std::stringstream& result) const
   {
      result << "bone name : " << boneAnim.boneName << std::endl;

      result << "scale: count = " << boneAnim.scaleKeysCount << "; keys = {" << std::endl;
      for (UINT i = 0; i < boneAnim.scaleKeysCount; ++i)
      {
         result << boneAnim.scaleKeysArr[i].Time << " - " << boneAnim.scaleKeysArr[i].Value << std::endl;
      }
      result << "};" << std::endl;

      result << "rotation: count = " << boneAnim.rotationKeysCount << "; keys = {" << std::endl;
      for (UINT i = 0; i < boneAnim.rotationKeysCount; ++i)
      {
         result << boneAnim.rotationKeysArr[i].Time << " - " << boneAnim.rotationKeysArr[i].Value << std::endl;
      }
      result << "};" << std::endl;

      result << "translation: count = " << boneAnim.translationKeysCount << "; keys = {" << std::endl;
      for (UINT i = 0; i < boneAnim.translationKeysCount; ++i)
      {
         result << boneAnim.translationKeysArr[i].Time << " - " << boneAnim.translationKeysArr[i].Value << std::endl;
      }
      result << "};" << std::endl;
   }
};

///////////////////////////////////////////////////////////////////////////////
