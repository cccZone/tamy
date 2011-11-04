/// @file   core-AI/SkeletonAnimationController.h
/// @brief  basic animation controller entity
#pragma once

#include "core-MVC/Entity.h"
#include "core/Node.h"
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
   DECLARE_CLASS( SkeletonAnimationController )

private:
   // static data
   SkeletonAnimation*                        m_source;

   // runtime data
   SpatialEntity*                            m_parent;
   std::vector< Node* >                      m_skeleton;
   float                                     m_trackTime;
   std::vector< BoneSRTAnimationPlayer* >    m_bonePlayers;
   std::vector< D3DXMATRIX >                 m_referenceMtcs;

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
    * Sets a new animation source.
    *
    * @param source           animation source
    */
   void setAnimationSource( SkeletonAnimation& source );

protected:
   // ----------------------------------------------------------------------
   // Object implementation
   // ----------------------------------------------------------------------
   void onPropertyChanged( Property& property );

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
