/// @file   TamyEditor/SkeletonAnimationEventsChart.h
/// @brief  skeleton animation events chart visualizer
#pragma once

#include <QGraphicsScene>


///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimation;

///////////////////////////////////////////////////////////////////////////////

/**
 * Skeleton animation keys chart visualizer.
 */
class SkeletonAnimationEventsChart : public QGraphicsScene
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
