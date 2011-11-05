/// @file   TamyEditor/AnimationTimeValueGetter.h
/// @brief  value getter for the animation chart time tracker
#pragma once

#include "VerticalChartMarker.h"
#include "core-AI/SkeletonAnimationController.h"
#include "AnimChartScales.h"


///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimationController;

///////////////////////////////////////////////////////////////////////////////

class AnimationTimeValueGetter : public VCMValueProvider
{
private:
   SkeletonAnimationController&              m_animationController;
   float                                     m_scale;

public:
   AnimationTimeValueGetter( SkeletonAnimationController& animationController, float scale )
      : m_animationController( animationController )
      , m_scale( scale )
   {
   }

   float getValue() const 
   {
      return m_animationController.getTrackTime() * m_scale;
   }
};

///////////////////////////////////////////////////////////////////////////////
