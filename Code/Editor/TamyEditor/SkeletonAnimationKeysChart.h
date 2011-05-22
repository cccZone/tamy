/// @file   TamyEditor/SkeletonAnimationKeysChart.h
/// @brief  skeleton animation keys chart visualizer
#pragma once

#include <QGraphicsScene>
#include <map>


///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimation;
class BoneSRTAnimationChart;
class BoneSRTAnimation;
class ChartLine;

///////////////////////////////////////////////////////////////////////////////

enum EBoneKeyId
{
   BAKEY_MIN = 0,
   BAKEY_POS_X = 0,
   BAKEY_POS_Y,
   BAKEY_POS_Z,
   BAKEY_YAW,
   BAKEY_PITCH,
   BAKEY_ROLL,
   BAKEY_MAX
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Skeleton animation keys chart visualizer.
 */
class SkeletonAnimationKeysChart : public QGraphicsScene
{
private:
   typedef std::map< std::string, BoneSRTAnimationChart* >  ChartsMap;

private:
   SkeletonAnimation&      m_animation;
   ChartsMap               m_charts;

   bool                    m_keysVisibility[BAKEY_MAX];
   QColor                  m_colors[BAKEY_MAX];
public:
   /**
    * Constructor.
    *
    * @param animation        displayed animation
    */
   SkeletonAnimationKeysChart( SkeletonAnimation& animation );
   ~SkeletonAnimationKeysChart();

   /**
    * Displays/hides the chart for the specified bone.
    *
    * @param boneName
    */
   void toggleBone( const std::string& boneName );

   /**
    * Checks if the specified bone is being displayed.
    *
    * @param boneName
    */
   bool isBoneDisplayed( const std::string& boneName ) const;

   /**
    * Displays/hides the specified animation key.
    *
    * @param keyIdx           bone animation key idx
    */
   void toggleKeyVisibility( EBoneKeyId keyIdx );

   /**
    * Checks if the specified animation key should be visible.
    *
    * @param keyIdx           bone animation key idx
    */
   inline bool isKeyVisible( EBoneKeyId keyIdx ) const { return m_keysVisibility[keyIdx]; }

   /**
    * Returns a color with which the specified color should be drawn.
    *
    * @param keyIdx           bone animation key idx
    */
   inline const QColor& getColor( EBoneKeyId keyIdx ) const { return m_colors[keyIdx]; }
};

///////////////////////////////////////////////////////////////////////////////

class BoneSRTAnimationChart
{
private:
   SkeletonAnimationKeysChart&      m_scene;
   BoneSRTAnimation&                m_boneAnimation;

   ChartLine*                       m_lines[BAKEY_MAX];

public:
   /**
    * Constructor.
    *
    * @param boneAnimation
    */
   BoneSRTAnimationChart( SkeletonAnimationKeysChart& scene, BoneSRTAnimation& boneAnimation );
   ~BoneSRTAnimationChart();

   /**
    * Called whenever the keys visibility changes.
    */
   void onKeysVisibilityChanged();

private:
   void updateVisibleKeys();
};

///////////////////////////////////////////////////////////////////////////////
