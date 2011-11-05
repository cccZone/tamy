/// @file   TamyEditor/SkeletonAnimationEventsChart.h
/// @brief  skeleton animation events chart visualizer
#pragma once

#include "ChartScene.h"


///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimation;

///////////////////////////////////////////////////////////////////////////////

/**
 * Skeleton animation keys chart visualizer.
 */
class SkeletonAnimationEventsChart : public ChartScene
{
private:
   SkeletonAnimation&      m_animation;

public:
   /**
    * Constructor.
    *
    * @param animation        displayed animation
    */
   SkeletonAnimationEventsChart( SkeletonAnimation& animation );
};

///////////////////////////////////////////////////////////////////////////////
