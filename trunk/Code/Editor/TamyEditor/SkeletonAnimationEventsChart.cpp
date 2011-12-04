#include "SkeletonAnimationEventsChart.h"
#include "core-AI/SkeletonAnimation.h"


///////////////////////////////////////////////////////////////////////////////

SkeletonAnimationEventsChart::SkeletonAnimationEventsChart()
   : m_animation( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationEventsChart::setAnimation( SkeletonAnimation& animation )
{
   m_animation = &animation;

   // go through the animation keys and draw proper lines showing them

}

///////////////////////////////////////////////////////////////////////////////
