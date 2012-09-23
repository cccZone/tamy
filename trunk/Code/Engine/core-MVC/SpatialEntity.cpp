#include "core-MVC\SpatialEntity.h"
#include "core\Assert.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( SpatialEntity )
   PARENT( Entity )
   PROPERTY_EDIT( "Local transformation", Matrix, m_localMtx )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

SpatialEntity::SpatialEntity( const std::string& name )
   : Entity( name )
   , Node( name )
{
}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity::SpatialEntity( const SpatialEntity& rhs )
   : Entity( rhs )
   , Node( rhs )
{
}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity::~SpatialEntity() 
{
   unsigned int count = m_children.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      SpatialEntity* se = DynamicCast< SpatialEntity >( m_children[i] );
      removeChild( *se );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SpatialEntity::onChildAttached( Entity& child ) 
{
   SpatialEntity* se = DynamicCast< SpatialEntity >( &child );
   if ( se == NULL ) 
   { 
      return; 
   }

   addChild( se );
}

///////////////////////////////////////////////////////////////////////////////

void SpatialEntity::onChildDetached( Entity& child )
{
   SpatialEntity* se = DynamicCast< SpatialEntity >( &child );
   if (se == NULL) 
   { 
      return; 
   }

   removeChild( *se );
}

///////////////////////////////////////////////////////////////////////////////

void SpatialEntity::onObjectLoaded()
{
   __super::onObjectLoaded();

   // update the node name
   setName( getEntityName() );
}

///////////////////////////////////////////////////////////////////////////////

Entity* SpatialEntity::cloneSelf() const
{
   SpatialEntity* entity = new SpatialEntity( *this );
   return entity;
}

///////////////////////////////////////////////////////////////////////////////
