#include "core-AI/SkeletonAnimationController.h"
#include "core-AI/SkeletonAnimation.h"
#include "core-AI/BoneSRTAnimation.h"
#include "core-MVC/SpatialEntity.h"
#include "core/Algorithms.h"
#include "core/Quaternion.h"
#include "core/Vector.h"
#include <list>
#include <set>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SkeletonAnimationController );
   PARENT( Entity );
   PROPERTY_EDIT( "Animation", SkeletonAnimation*, m_source );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

SkeletonAnimationController::SkeletonAnimationController( const std::string& name )
   : Entity( name )
   , m_source( NULL )
   , m_parent( NULL )
   , m_trackTime( 0.f )
   , m_pause( false )
{
}

///////////////////////////////////////////////////////////////////////////////

SkeletonAnimationController::SkeletonAnimationController( const SkeletonAnimationController& rhs )
   : Entity( rhs )
   , m_source( rhs.m_source )
   , m_parent( NULL )
   , m_trackTime( 0.f )
   , m_pause( false )
{

}

///////////////////////////////////////////////////////////////////////////////

SkeletonAnimationController::~SkeletonAnimationController()
{
   m_source = NULL;
   m_parent = NULL;
   
   m_referenceMtcs.clear();
   m_skeleton.clear();

   // clear the runtime play data
   unsigned int count = m_bonePlayers.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_bonePlayers[i];
   }
   m_bonePlayers.clear();
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::setAnimationSource( SkeletonAnimation& source )
{
   // set the new animation source
   m_source = &source;

   onDataChanged();
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_source" )
   {
      onDataChanged();
   }
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::onAttached( Entity& parent )
{
   m_parent = DynamicCast< SpatialEntity >( &parent );
   onDataChanged();
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::onDetached( Entity& parent ) 
{
   m_parent = NULL;
   onDataChanged();
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::onUpdate( float timeElapsed ) 
{
   unsigned int count = m_skeleton.size();
   if ( m_source == NULL || count == 0 )
   {
      // don't bother if there's no animation data source defined
      // or if there's no skeleton attached
      return;
   }

   // update the track time ( only if the controller's not paused )
   if ( !m_pause )
   {
      m_trackTime += timeElapsed;
      const float animLength = m_source->getAnimationLength();
      while ( m_trackTime > animLength )
      {
         m_trackTime -= animLength;
      }
   }

   // Go through the nodes and query the update matrices for them,
   // updating their local matrices with those
   Matrix updateMtx, tmpMtx;
   Quaternion orientation;
   Vector translation;
   for ( unsigned int i = 0; i < count; ++i )
   {
      BoneSRTAnimationPlayer* player = m_bonePlayers[i];
      if ( player == NULL )
      {
         // there's no bone in the animation definition corresponding
         // to the one in the skeleton - skip it
         continue;
      }

      if ( player->getOrientation( m_trackTime, orientation ) )
      {
         updateMtx.setRotation( orientation );
      }
      else
      {
         updateMtx = Matrix::IDENTITY;
      }

      if ( player->getTranslation( m_trackTime, translation ) )
      {
         tmpMtx.setTranslation( translation );
         updateMtx.mul( tmpMtx );
      }
      
      updateMtx.mul( m_referenceMtcs[i] );
      m_skeleton[i]->setLocalMtx( updateMtx );
   }

   // <animation.todo> creating a Transform class that contains only the rotation Quaternion and a position Vector
   // will remove the need to multiply the matrices so many times, thus speeding this code 200%, and
   // will reduce the memory footprint
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::setTrackTime( float time )
{
   m_trackTime = clamp( time, 0.0f, m_source->getAnimationLength() );
}

///////////////////////////////////////////////////////////////////////////////

Entity* SkeletonAnimationController::cloneSelf() const
{
   SkeletonAnimationController* entity = new SkeletonAnimationController( *this );
   return entity;
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::onDataChanged()
{
   // reset the skeleton to the reference pose
   unsigned int count = m_skeleton.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_skeleton[i]->setLocalMtx( m_referenceMtcs[i] );
   }
   m_referenceMtcs.clear();

   // clear the skeleton definition
   m_skeleton.clear();

   // clear the runtime play data
   count = m_bonePlayers.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_bonePlayers[i];
   }
   m_bonePlayers.clear();


   // if we have a parent node and an animation source, we can try parsing the skeleton
   if ( m_parent && m_source )
   {
      // iterate over the hierarchy of nodes and parse the skeleton nodes
      std::list< SpatialEntity* > nodesQueue;
      std::set< std::string > usedBoneNames;

      // The parent must be a root spatial entity - otherwise
      // we don't parse.
      // We can't allow for a parent's child to be a root node due to the following reasons:
      //  - many of the children could be spatial entities, so we wouldn't know which one to select
      //    as a root
      //  - we have a direct link only to the parent - we get notifications about changes it undergoes etc.
      //    but we don't have the same for its children - that's why it would be difficult to track
      //    if such a child was detached for instance.
      //  - a child operating in the scope of its parent is a supported architectural 
      //    choice in this engine

      // parse the nodes in the queue
      nodesQueue.push_back( m_parent );
      while( nodesQueue.empty() == false )
      {
         SpatialEntity* currNode = nodesQueue.front();
         nodesQueue.pop_front();

         const std::string& boneName = currNode->getName();
         if ( usedBoneNames.find( boneName ) != usedBoneNames.end() )
         {
            // the bone with this name is already used - don't map it
            continue;
         }

         // Add the node to the skeleton and memorize its name.
         // We're using the node's name, although from the scope of a spatial entity
         // this and the entity's name are guaranteed to be the same
         m_skeleton.push_back( currNode );
         usedBoneNames.insert( currNode->getName() );

         // memorize node's local matrix
         m_referenceMtcs.push_back( currNode->getLocalMtx() );

         // go through all the children and add those that are spatial entities
         const Entity::Children& children = currNode->getEntityChildren();
         unsigned int count = children.size();
         for( unsigned int i = 0; i < count; ++i )
         {
            SpatialEntity* spatialEntity = DynamicCast< SpatialEntity >( children[i] );
            if ( spatialEntity != NULL )
            {
               nodesQueue.push_back( spatialEntity );
            }
         }
      }

      if ( m_skeleton.empty() == false )
      {
         m_source->initializePlayer( m_skeleton, m_bonePlayers );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
