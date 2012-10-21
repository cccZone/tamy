/// @file   core-AI/SkeletonAnimationController.h
/// @brief  basic animation controller entity
#pragma once

#include "core-MVC\Entity.h"
#include "core\Node.h"
#include "core\Matrix.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class SkeletonAnimation;
class BoneSRTAnimationPlayer;
class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * An entity responsible for animating a hierarchy of nodes.
 *
 * The hierarchy of nodes it will animate is supplied by the parent
 * entity it gets attached to.
 */
class SkeletonAnimationController : public Entity
{
   DECLARE_ALLOCATOR( SkeletonAnimationController, AM_DEFAULT );
   DECLARE_CLASS()

private:
   // static data
   SkeletonAnimation*                        m_source;

   // runtime data
   SpatialEntity*                            m_parent;
   std::vector< Node* >                      m_skeleton;
   std::vector< BoneSRTAnimationPlayer* >    m_bonePlayers;
   Array< Matrix >                           m_referenceMtcs;
   float                                     m_trackTime;
   bool                                      m_pause;
 
public:
   /**
    * Constructor.
    */
   SkeletonAnimationController( const std::string& name = "" );

   /**
    * Copy constructor.
    */
   SkeletonAnimationController( const SkeletonAnimationController& rhs );
   ~SkeletonAnimationController();

   /**
    * Returns the current track time.
    */
   inline float getTrackTime() const { return m_trackTime; }

   /**
    * Sets the new track time.
    *
    * @param time
    */
   void setTrackTime( float time );

   /**
    * Toggles a pause.
    */
   void pause( bool enable ) { m_pause = enable; }

   /**
    * Sets a new animation source.
    *
    * @param source           animation source
    */
   void setAnimationSource( SkeletonAnimation& source );

protected:
   // ----------------------------------------------------------------------
   // Object implementation
   // ----------------------------------------------------------------------
   void onPropertyChanged( ReflectionProperty& property );

   // ----------------------------------------------------------------------
   // Entity implementation
   // ----------------------------------------------------------------------
   void onAttached( Entity& parent );
   void onDetached( Entity& parent );
   void onUpdate( float timeElapsed );
   Entity* cloneSelf() const;

private:
   void onDataChanged();
};

///////////////////////////////////////////////////////////////////////////////
