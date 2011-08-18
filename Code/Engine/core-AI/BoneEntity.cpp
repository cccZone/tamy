#include "core-AI/BoneEntity.h"
#include "core/Color.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/DebugDrawCommands.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( BoneEntity, SpatialEntity )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

BoneEntity::BoneEntity( const std::string& boneName )
   : SpatialEntity( boneName )
   , m_debugDraw( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

BoneEntity::~BoneEntity()
{
}

///////////////////////////////////////////////////////////////////////////////

void BoneEntity::onDebugRender( Renderer& renderer ) const
{
   SpatialEntity* parentNode = NULL;
   if ( isAttached() )
   {
      parentNode = dynamic_cast< SpatialEntity* >( &Entity::getParent() );
   }

   if ( parentNode )
   {
      D3DXVECTOR3 start[4];
      D3DXVECTOR3 end[4];
      getGlobalVectors( start[0], start[1], start[2], start[3] );
      parentNode->getGlobalVectors( end[0], end[1], end[2], end[3] );

      new ( renderer() ) RCDrawLine( start[3], end[3], Color( 0, 0, 0 ) );
   }
}

///////////////////////////////////////////////////////////////////////////////