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
   SkeletonAnimation*      m_animation;

public:
   /**
    * Constructor.
    */
   SkeletonAnimationEventsChart();

   /**
    * Sets a new animation for the chart to use.
    *
    * @param animation
    */
   void setAnimation( SkeletonAnimation& animation );
};

///////////////////////////////////////////////////////////////////////////////
