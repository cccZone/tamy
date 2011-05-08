#include "core-AI/SkeletonAnimationController.h"
#include "core-AI/SkeletonAnimation.h"
#include "core-AI/BoneSRTAnimation.h"
#include "core-MVC/SpatialEntity.h"
#include <list>
#include <set>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SkeletonAnimationController, Entity );
   PROPERTY_EDIT( "Animation", SkeletonAnimation*, m_source );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

SkeletonAnimationController::SkeletonAnimationController( const std::string& name )
   : Entity( name )
   , m_source( NULL )
   , m_trackTime( 0.f )
{
}

///////////////////////////////////////////////////////////////////////////////

SkeletonAnimationController::~SkeletonAnimationController()
{
   unsigned int count = m_bonePlayers.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_bonePlayers[i];
   }
   m_bonePlayers.clear();

   m_source = NULL;

   m_skeleton.clear();
   m_referenceMtcs.clear();
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::setAnimationSource( SkeletonAnimation& source )
{
   // set the new animation source
   m_source = &source;

   // initialize the play data
   unsigned int count = m_bonePlayers.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_bonePlayers[i];
   }
   m_bonePlayers.clear();

   if ( m_source != NULL && m_skeleton.empty() == false )
   {
      m_source->initializePlayer( m_skeleton, m_bonePlayers );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::onAttached( Entity& parent )
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
   SpatialEntity* spatialEntity = dynamic_cast< SpatialEntity* >( &parent );
   if ( spatialEntity != NULL )
   {
      nodesQueue.push_back( spatialEntity );
   }
   else
   {
      return;
   }

   // clean the present skeleton definition
   m_skeleton.clear();
   m_referenceMtcs.clear();

   // parse the nodes in the queue
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
         spatialEntity = dynamic_cast< SpatialEntity* >( children[i] );
         if ( spatialEntity != NULL )
         {
            nodesQueue.push_back( spatialEntity );
         }
      }
   }

   // create the runtime play data
   unsigned int count = m_bonePlayers.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_bonePlayers[i];
   }
   m_bonePlayers.clear();

   if ( m_source != NULL && m_skeleton.empty() == false )
   {
      m_source->initializePlayer( m_skeleton, m_bonePlayers );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SkeletonAnimationController::onDetached( Entity& parent ) 
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

   // update the track time
   m_trackTime += timeElapsed;
   const float animLength = m_source->getAnimationLength();
   while ( m_trackTime > animLength )
   {
      m_trackTime -= animLength;
   }

   // Go through the nodes and query the update matrices for them,
   // updating their local matrices with those
   D3DXMATRIX updateMtx, tmpMtx;
   D3DXQUATERNION orientation;
   D3DXVECTOR3 translation;
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
         D3DXMatrixRotationQuaternion( &updateMtx, &orientation );
      }
      else
      {
         D3DXMatrixIdentity( &updateMtx );
      }

      if ( player->getTranslation( m_trackTime, translation ) )
      {
         D3DXMatrixTranslation( &tmpMtx, translation.x, translation.y, translation.z );
         D3DXMatrixMultiply( &updateMtx, &updateMtx, &tmpMtx );
      }
      
      D3DXMatrixMultiply( &tmpMtx, &updateMtx, &( m_referenceMtcs[i] ) );
      m_skeleton[i]->setLocalMtx( tmpMtx );
   }
}

///////////////////////////////////////////////////////////////////////////////
